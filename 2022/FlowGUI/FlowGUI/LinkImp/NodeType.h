#ifndef FLOWGUI_NODETYPE_H
#define FLOWGUI_NODETYPE_H

//FlowGUI
#include "BlockImp/PortType.h"

//std
//#include <utility>

//Qt
#include <QPointF>

namespace FlowGUI {
struct PortType;
class NodeType
{
private:
    using Idx   = uint32_t;
    static constexpr Idx invalid_index = ~Idx(0);
public:
//    NodeType();
//    NodeType(const Coord &coord   ,
//             const Idx   &linkTo  = invalid_index,
//                   Port  *port    = nullptr);

    //rule of 5 must be applied because
    //the port connection is treated as a resource
    //that must be moved or freed
    //[IMPORTANT]: follow Core Guideline C.66:
    //Make move operations noexcept
    //(applying C.66 will allow std::vector to move
    //instead of copy when resizing)
//    NodeType(const NodeType &ext);
//    NodeType(NodeType &&ext) noexcept;
//    NodeType &operator=(const NodeType &ext);
//    NodeType &operator=(NodeType &&ext) noexcept;
//    ~NodeType();
//
//    bool isSelected() const;
//    void select();
//    void diselect();
//    bool isEmpty()    const;
//    void makeEmpty();
//
//    void connectToPort(Port *port);
//    void disconnectFromPort();

    NodeType()
        : coord(QPointF())
        , linkTo(invalid_index)
        , status({.empty=true,.selected=false,.childs=0})
        , port(nullptr) {}
    NodeType(const QPointF&  coord  ,
             const Idx&      linkTo = invalid_index ,
                   PortType* port   = nullptr )
        : coord(coord)
        , linkTo(linkTo)
        , status({.empty=false,.selected=false,.childs=0})
        , port(port) {}
    NodeType(const NodeType &ext)
        : coord(ext.coord)
        , linkTo(ext.linkTo)
        , status(ext.status)
        , port(ext.port) {}
    NodeType(NodeType &&ext) noexcept
        : coord (std::move(ext.coord))
        , linkTo(std::move(ext.linkTo))
        , status(std::move(ext.status))
        , port  (std::move(ext.port))
    //    , port  (std::exchange(ext.port,nullptr))
    {
        ext.port = nullptr; //this can be replaced for std::exchange

        //[OPTIONAL] As Core Guidelines C.64 suggest,
        //leave the moved-from object in a default state
        ext.makeEmpty();
    }
    NodeType& operator=(const NodeType &ext)
    {
        //avoid self copy
        if( this == &ext )
            return *this;
        //1- clean this
        disconnectFromPort();
        //2- copy
        coord  = ext.coord;
        linkTo = ext.linkTo;
        status = ext.status;
        port   = ext.port;
        return *this;
    }
    NodeType& operator=(NodeType&& ext) noexcept
    {
        //avoid move to itself
        if( this == &ext )
            return *this;
        //1- clean this
        disconnectFromPort();
        //2- move
        coord  = std::move(ext.coord);
        linkTo = std::move(ext.linkTo);
        status = std::move(ext.status);
        port   = std::move(ext.port);
        //3- clean extern and (OPTIONAL) let it in a defaul defined state
        ext.port = nullptr;
        //[OPTIONAL] As Core Guidelines C.64 suggest:
        //leave the moved-from object in a default state
        ext.makeEmpty();

        return *this;
    }
    ~NodeType()
    {
        makeEmpty();
    }
    bool isSelected() const
    {
        return status.selected;
    }
    void select()
    {
        status.selected = true;
    }
    void deselect()
    {
        status.selected = false;
    }
    bool isEmpty() const
    {
        return status.empty;
    }
    void makeEmpty()
    {
        coord  = QPointF();
        linkTo = invalid_index;
        status.empty    = true;
        status.selected = false;
        status.childs = 0;
        disconnectFromPort();
    }
    void connectToPort(PortType *port)
    {
        this->port = port;
    //    #warning "call methods to inform the port about the connection"
        //port->connect();
    }
    void disconnectFromPort()
    {
        if( port == nullptr )
            return;
    //    #warning "call methods to inform the port that is not longer connected"
        //port->disconnect();
        port = nullptr;
    }

    //constexpr uint32_t invalid_index = ~uint32_t(0); //all bits high
    //using Node = NodeType<QPointF,int,uint32_t,invalid_index>;


public:
    //no need to devel setter/getter on this attributes, because
    //there is no checks to apply before assign the arguments to
    //the member variables (attributes)
    QPointF coord;
    Idx     linkTo;
    struct
    {
        Idx empty    : 1;
        Idx selected : 1;
        Idx childs   : (sizeof(Idx)*8-2);//the 2 is because empty + selected
    }status;
private:
    //[TODO]: should use a std::unique_ptr instead of raw Port* ??
//    #warning "should use a std::unique_ptr instead of raw Port* ??"
    PortType* port;
};
/*
proto()::NodeType()
    : coord(Coord())
    , linkTo(invalid_index)
    , status({.empty=true,.selected=false,.childs=0})
    , port(nullptr)
{}

proto()::NodeType(const Coord &coord  ,
                  const Idx   &linkTo ,
                        Port  *port   )
    : coord(coord)
    , linkTo(linkTo)
    , status({.empty=false,.select=false,.childs=0})
    , port(port)
{}

proto()::NodeType(const NodeType &ext)
    : coord(ext.coord)
    , linkTo(ext.linkTo)
    , status(ext.status)
    , port(ext.port)
{}

proto()::NodeType(NodeType &&ext) noexcept
    : coord (std::move(ext.coord))
    , linkTo(std::move(ext.linkTo))
    , status(std::move(ext.status))
    , port  (std::move(ext.port))
//    , port  (std::exchange(ext.port,nullptr))
{
    ext.port = nullptr; //this can be replaced for std::exchange

    //[OPTIONAL] As Core Guidelines C.64 suggest,
    //leave the moved-from object in a default state
    ext.makeEmpty();
}

proto(proto_instance &)::operator=(const proto_instance &ext)
{
    //avoid self copy
    if( this == &ext )
        return *this;
    //1- clean this
    disconnectFromPort();
    //2- copy
    coord  = ext.coord;
    linkTo = ext.linkTo;
    status = ext.status;
    port   = ext.port;
    return *this;
}

proto(proto_instance &)::operator=(proto_instance &&ext) noexcept
{
    //avoid move to itself
    if( this == &ext )
        return *this;
    //1- clean this
    disconnectFromPort();
    //2- move
    coord  = std::move(ext.coord);
    linkTo = std::move(ext.linkTo);
    status = std::move(ext.status);
    port   = std::move(ext.port);
    //3- clean extern and (OPTIONAL) let it in a defaul defined state
    ext.port = nullptr;
    //[OPTIONAL] As Core Guidelines C.64 suggest:
    //leave the moved-from object in a default state
    ext.makeEmpty();

    return *this;
}

proto()::~NodeType()
{
    makeEmpty();
}

proto(bool)::isSelected() const
{
    return status.selected;
}

proto(void)::select()
{
    status.selected = true;
}

proto(void)::diselect()
{
    status.selected = false;
}


proto(bool)::isEmpty() const
{
    return status.empty;
}

proto(void)::makeEmpty()
{
    coord  = Coord();
    linkTo = invalid_index;
    status.empty    = true;
    status.selected = false;
    status.childs = 0;
    disconnectFromPort();
}

proto(void)::connectToPort(Port *port)
{
    this->port = port;
//    #warning "call methods to inform the port about the connection"
    //port->connect();
}

proto(void)::disconnectFromPort()
{
    if( port == nullptr )
        return;
//    #warning "call methods to inform the port that is not longer connected"
    //port->disconnect();
    port = nullptr;
}

//constexpr uint32_t invalid_index = ~uint32_t(0); //all bits high
//using Node = NodeType<QPointF,int,uint32_t,invalid_index>;

*/
#undef proto_header
#undef proto_instance
#undef proto

} // namespace FlowGUI

#endif // FLOWGUI_NODETYPE_H
