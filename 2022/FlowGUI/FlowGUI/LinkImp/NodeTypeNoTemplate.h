#ifndef FLOWGUI_NODETYPENOTEMPLATE_H
#define FLOWGUI_NODETYPENOTEMPLATE_H

//FlowGUI

//std
//#include <utility>

//Qt
#include <QPointF>
#include <cstdint>

namespace FlowGUI {

#define proto_instance NodeType
#define proto(ret_type) inline ret_type NodeType
class NodeType
{
public:
    using Coord = QPointF;
    using Port  = struct Port{};
    using Idx   = uint32_t;
    static constexpr Idx invalid_index = ~Idx(0); //all bits high

public:
    NodeType();
    NodeType(const Coord &coord   ,
             const Idx   &linkTo  = invalid_index,
                   Port  *port    = nullptr);

    //rule of 5 must be applied because
    //the port connection is treated as a resource
    //that must be moved or freed
    //[IMPORTANT]: follow Core Guideline C.66:
    //Make move operations noexcept
    //(applying C.66 will allow std::vector to move
    //instead of copy when resizing)
    NodeType(const NodeType &ext);
    NodeType(NodeType &&ext) noexcept;
    NodeType &operator=(const NodeType &ext);
    NodeType &operator=(NodeType &&ext) noexcept;
    ~NodeType();

    bool isEmpty() const;
    void makeEmpty();

    void connectToPort(Port *port);
    void disconnectFromPort();

public:
    //no need to devel setter/getter on this attributes, because
    //there is no checks to apply before assign the arguments to
    //the member variables (attributes)
    Coord coord;
    Idx   linkTo;
    struct
    {
        Idx empty  : 1;
        Idx childs : (sizeof(Idx)*8-1);
    }status;
private:
    //[TODO]: should use a std::unique_ptr instead of raw Port* ??
    #warning "should use a std::unique_ptr instead of raw Port* ??"
    Port  *port;
};

proto()::NodeType()
    : coord(Coord())
    , linkTo(invalid_index)
    , status({.empty=true,.childs=0})
    , port(nullptr)
{}

proto()::NodeType(const Coord &coord  ,
                  const Idx   &linkTo ,
                        Port  *port   )
    : coord(coord)
    , linkTo(linkTo)
    , status({.empty=false,.childs=0})
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

proto(bool)::isEmpty() const
{
    return status.empty;
}

proto(void)::makeEmpty()
{
    coord  = Coord();
    linkTo = invalid_index;
    status.empty  = true;
    status.childs = 0;
    disconnectFromPort();
}

proto(void)::connectToPort(Port *port)
{
    this->port = port;
    #warning "call methods to inform the port about the connection"
    //port->connect();
}

proto(void)::disconnectFromPort()
{
    if( port == nullptr )
        return;
    #warning "call methods to inform the port that is not longer connected"
    //port->disconnect();
    port = nullptr;
}

#undef proto_header
#undef proto_instance
#undef proto

} // namespace FlowGUI

#endif // FLOWGUI_NODETYPENOTEMPLATE_H
