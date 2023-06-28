//FlowGUI
#include "BlockImp/PortType.h"
#include "Style.h"

//std

//Qt

namespace FlowGUI {

PortType::PortType()
    : _typeName()
    , _instanceName()
    , _connectedNode(nullptr) {}

PortType::PortType(const QString &typeName,
           const QString &instanceName,
           NodeType *node)
    : _typeName(typeName)
    , _instanceName(instanceName)
    , _connectedNode(node)
{
    if( _connectedNode != nullptr )
        _connectedNode->connectToPort(this);
//    auto size = StyleBlockShape::connectorSizeGridSizePercent*StyleGrid::gridSize;
//    _shape.moveTo(0.0,-size.height()/2.0);
//    _shape.lineTo(size.width(),0.0);
//    _shape.lineTo(0.0, size.height()/2.0);
}

void PortType::updateConnectionPos(const QPointF &pos)
{
    _connectionPos = pos;
//    if( _connectedNode != nullptr )
//        _connectedNode->moveTo(pos);
}

} // namespace FlowGUI
