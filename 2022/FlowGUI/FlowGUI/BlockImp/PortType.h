#ifndef FLOWGUI_PORTINTERFACE_H
#define FLOWGUI_PORTINTERFACE_H

//FlowGUI
#include "LinkImp/NodeType.h"

//std

//Qt
#include <QPainterPath>
#include <QString>

namespace FlowGUI {

class NodeType;
struct PortType
{
public:
    PortType();
    PortType(const QString& typeName,
             const QString& instanceName,
             NodeType* node = nullptr);
//    PortType() = default;
//    PortType(PortType&& ) = default;
//    PortType(const PortType& ) = default;
//    PortType& operator=(PortType&&) = default;
//    PortType& operator=(PortType&) = default;
//    ~PortType() = default;

    void updateConnectionPos(const QPointF& pos);
    QString getType() const { return _typeName; }
    QString getInstance() const { return _instanceName; }

private:
    QString   _typeName;
    QString   _instanceName;
    NodeType* _connectedNode = nullptr;
    QPointF   _connectionPos;
    //QPainterPath _shape;
};

} // namespace FlowGUI

#endif // FLOWGUI_PORTINTERFACE_H
