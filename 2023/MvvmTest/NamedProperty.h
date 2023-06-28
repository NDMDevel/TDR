#pragma once

#include <utility>
#include <string>

template<typename T>
class NamedProperty
{
public:
    NamedProperty(std::string name,const T& value = T{})
        : prop({name,value}) {}

    std::string name()  const { return prop.first; }
    T& value()                { return prop.second; }
    T value()           const { return prop.second; }

    std::pair<const std::string,T> prop;
};
