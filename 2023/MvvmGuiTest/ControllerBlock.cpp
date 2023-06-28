#include "ControllerBlock.h"
#include "ViewModelBlock.h"

ControllerProperty operator|(ControllerProperty::ID arg1,ControllerProperty::ID arg2)
{
    ControllerProperty retval(arg1);
    retval.set(arg2);
    return retval;
}
ControllerProperty operator|(ControllerProperty prop,ControllerProperty::ID arg)
{
    prop.set(arg);
    return prop;
}
ControllerProperty operator|(ControllerProperty::ID arg,ControllerProperty prop)
{
    prop.set(arg);
    return prop;
}


ControllerBlock::ControllerBlock(ModelBlock& model)
    : _model(model)
{

}
std::weak_ptr<ViewModelBlock> ControllerBlock::addViewModel()
{
//    _vms.emplace_back(std::make_shared<ViewModelBlock>(*this));
//    return *(_vms.begin()+_vms.size()-1);//_vmObservers.front();
}
void ControllerBlock::setName(const QString& name,ViewModelBlock* origin) const
{
    //updates model and notify all view models
    _model.name = name;
    updateViewModels(ControllerProperty::ID::NAME,origin);
}
QString ControllerBlock::getName() const { return _model.name; }
void ControllerBlock::setType(const QString &type, ViewModelBlock *origin) const
{
    _model.type = type;
    updateViewModels(ControllerProperty::ID::TYPE,origin);
}
QString ControllerBlock::getType() const { return _model.type; }
void ControllerBlock::updateViewModels(ControllerProperty propsChanged,ViewModelBlock* exclude) const
{
    for( auto& vm : _vms )
        if( vm.get() != exclude );
//            vm->properiesChanged(propsChanged);
}
