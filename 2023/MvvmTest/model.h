#pragma once

//#include "bindey/property.h"
#include "NamedProperty.h"
#include <iostream>
#include <variant>
#include <cstdint>
#include <string>
#include <vector>

using PropType = std::variant<std::string,
                              uint64_t,
                              int64_t,
                              double,
                              bool>;

class Model
{
public:
    template<typename T>
    bool addProp(const std::string& name,
                 const T& value);

    void showPropertiesName() const;
    void showAll() const;

public:


public:
    std::vector<NamedProperty<PropType>> _props;
};

template<typename T>
bool Model::addProp(const std::string& name,
                    const T& value)
{
/*
This method adds a new property to a std::vector<NamedProperty>.
NamedProperty is a simple wrapper to std::pair<std::string,std::variant>;
the std::string represents the name and the std::variant the type of the property.

The variant valid types are:
    std::string
    uint64_t
    int64_t
    double
    bool

A problem will arise when trying to add a new property of type int, float, or
a const char* string, etc; in general, any type not supported by the variant.
Even though int, float and const char* are convertible to int64_t, double
and std::string respectively, that conversion must be done explicitly before
assigning the value to the variant.

So we need to detect when the type is convertible to one of the supported ones (by
the variant) and do the conversion explicitly.
*/

    //For each valid variant type an alias will be created:
    //  uint
    //  sint
    //  real
    //  strt
    //  Bool
    //Each alias will take a valid variant type or void depending
    //of T being convertible to a variant type.
    //For instance, if T is convertible to double, then real will
    //alias double (not void) and all the rest of the alias will
    //be void.
    using uint = std::conditional_t<   std::is_same_v<T,uint8_t>
                                    || std::is_same_v<T,uint16_t>
                                    || std::is_same_v<T,uint32_t>
                                    || std::is_same_v<T,uint64_t>
                                    || std::is_same_v<T,unsigned long long int>
                                    , uint64_t,void>;
    using sint = std::conditional_t<   std::is_same_v<T,int8_t>
                                    || std::is_same_v<T,int16_t>
                                    || std::is_same_v<T,int32_t>
                                    || std::is_same_v<T,int64_t>
                                    || std::is_same_v<T,long long int>
                                    , int64_t,void>;
    using real = std::conditional_t<   std::is_same_v<T,float>
                                    || std::is_same_v<T,double>
                                    || std::is_same_v<T,long double>
                                    , double,void>;
    using strt = std::conditional_t<   std::is_same_v<T,std::string>
                                    || std::is_same_v<T,char*>
                                    || std::is_same_v<T,const char*>
                                    || std::is_same_v<std::remove_all_extents_t<T>,char>
                                    ,std::string,void>;
    using Bool = std::conditional_t<   std::is_same_v<T,bool>
                                    ,bool,void>;

    //Type will take the type of the previous alias that is not void.
    //If all the alias are void (meaning T is not supported by the variant)
    //then Type will be void.
    using Type = std::conditional_t<!std::is_same_v<uint,void>,uint,
                 std::conditional_t<!std::is_same_v<sint,void>,sint,
                 std::conditional_t<!std::is_same_v<real,void>,real,
                 std::conditional_t<!std::is_same_v<strt,void>,strt,
                 std::conditional_t<!std::is_same_v<Bool,void>,Bool,void>>>>>;

    //if Type is void, then generate an error, otherwise use Type to
    //explicitly convert T to Type.
    static_assert(!std::is_same_v<Type,void>,
                  "T must be integer (signed or unsigned), floating point, bool or string");

    //if the property name alread exists, return false
    for( const auto& n : _props )
        if( n.name() == name )
            return false;   //failed to add the property

    //add the property by converting T (the type of value) to Type (a supporte type
    //by the variant):
    _props.push_back({name,Type(value)});

    return true;    //all ok
}
