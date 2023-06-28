#ifndef CONTROLLERBLOCK_H
#define CONTROLLERBLOCK_H

#include <vector>
#include <memory>
#include "ModelBlock.h"
//#include "ViewModelBlock.h"
//#include "nod.hpp"

class ViewModelBlock;

class ControllerProperty
{
public:
    enum class ID: uint64_t
    {
        NONE = 0,
        NAME = uint64_t((uint64_t(1))<<0),
        TYPE = uint64_t((uint64_t(1))<<1),

        ALL  = uint64_t((uint64_t(1))<<63)
    };
public:
    ControllerProperty():_container(ID::NONE){}
    ControllerProperty(ID arg):_container(arg){}

    void set(ID prop)
    {
        auto temp = toRaw(_container);
        temp |= toRaw(prop);
        _container = toID(temp);
    }
    void clear(ID prop)
    {
        auto temp = toRaw(_container);
        temp &= ~toRaw(prop);
        _container = toID(temp);
    }
    bool get(ID prop) const
    {
        auto mask = toRaw(prop);
        auto temp = toRaw(_container);
        return (temp&mask) != 0;
    }
    ID   pop()
    {
        if( _container == ID::NONE )
            return ID::NONE;
        auto mask = toRaw(ID::NAME);
        auto temp = toRaw(_container);
        while( (mask&temp) == 0 )
            mask <<= 1;
        ID retval = toID(mask);
        clear(retval);
        return retval;
    }
private:
    uint64_t toRaw(ID prop)const    { return static_cast<uint64_t>(prop); }
    ID       toID(uint64_t raw)const{ return static_cast<ID>(raw); }
private:
    ID _container;
};

ControllerProperty operator|(ControllerProperty::ID arg1,ControllerProperty::ID arg2);
ControllerProperty operator|(ControllerProperty prop,ControllerProperty::ID arg);
ControllerProperty operator|(ControllerProperty::ID arg,ControllerProperty prop);

class ControllerBlock
{
public:
    ControllerBlock(ModelBlock& model);
    std::weak_ptr<ViewModelBlock> addViewModel();
//    void removeViewModel(const std::weak_ptr<ViewModelBlock>& vm);

    void    setName(const QString& name,ViewModelBlock* origin=nullptr) const;
    QString getName() const;
    void    setType(const QString& type,ViewModelBlock* origin=nullptr) const;
    QString getType() const;

protected:
    void updateViewModels(ControllerProperty propsChanged=ControllerProperty::ID::ALL,ViewModelBlock* exclude=nullptr) const;

private:
    ModelBlock& _model;
    std::vector<std::shared_ptr<ViewModelBlock>> _vms;
//    nod::signal<void(const QString&)> onNameChanged;
//    nod::signal<void(const QString&)> onTypeChanged;
};

#endif // CONTROLLERBLOCK_H
