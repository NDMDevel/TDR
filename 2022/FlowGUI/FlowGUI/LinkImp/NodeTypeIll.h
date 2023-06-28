#ifndef FLOWGUI_NODETYPEILL_H
#define FLOWGUI_NODETYPEILL_H

//FlowGUI

//std
#include <cstdint>

//Qt

namespace FlowGUI {

//helper MACROS
#define proto_header        \
    template<               \
        typename Idx  ,     \
        typename Coord,     \
        typename Port ,     \
        Idx invalid_index>

#define proto_instance NodeTypeIll<Idx,Coord,Port,invalid_index>

#define proto(ret_val)     \
    proto_header           \
    ret_val proto_instance

proto_header
class NodeTypeIll
{
public:
    NodeTypeIll();
    NodeTypeIll(const Coord &coord ,
             const Idx   &prev  = invalid_index,
             const Idx   &nextk = invalid_index,
                   Port  *port  = nullptr);

    //rule of 5 must be applied
    NodeTypeIll(const NodeTypeIll &ext);
    NodeTypeIll(NodeTypeIll &&ext);
    NodeTypeIll &operator=(const NodeTypeIll &ext);
    NodeTypeIll &operator=(NodeTypeIll &&ext);
    ~NodeTypeIll();

    bool isEmpty() const;
    void makeEmpty();

    void connectToPort(Port *port);
    void disconnectFromPort();

public:
    //no need to devel setter/getter on this attributes, because
    //there is no checks to apply before assign the arguments to
    //the member variables (attributes)
    Coord coord;
    Idx   prev;
    Idx   next;
private:
    bool  empty;
    Port  *port;
};

proto()::NodeTypeIll()
    : coord()
    , prev(invalid_index)
    , next(invalid_index)
    , empty(true)
    , port(nullptr)
{}

proto()::NodeTypeIll(const Coord &coord ,
                  const Idx   &prev  ,
                  const Idx   &next  ,
                        Port  *port  )
    : coord(coord)
    , prev(prev)
    , next(next)
    , empty(false)
    , port(port)
{}

proto()::NodeTypeIll(const proto_instance &ext)
{
    coord = ext.coord;
    prev  = ext.prev;
    next  = ext.next;
    empty = ext.empty;
    port  = ext.port;
}

proto()::NodeTypeIll(proto_instance &&ext)
{
    coord = ext.coord;
    prev  = ext.prev;
    next  = ext.next;
    empty = ext.empty;

    //transfer resource from ext to this
    port  = ext.port;
    ext.port = nullptr;
}

proto(proto_instance &)::operator=(const proto_instance &ext)
{
    coord = ext.coord;
    prev  = ext.prev;
    next  = ext.port;
    empty = ext.empty;
    port  = ext.port;
    return *this;
}

proto(proto_instance &)::operator=(proto_instance &&ext)
{
    coord = ext.coord;
    prev  = ext.prev;
    next  = ext.next;
    empty = ext.empty;
    //transfer resource from ext to this
    port  = ext.port;
    ext.port = nullptr;
    return *this;
}

proto()::~NodeTypeIll()
{
    makeEmpty();
}

proto(bool)::isEmpty() const
{
    return empty;
}

proto(void)::makeEmpty()
{
    coord = Coord();
    prev  = invalid_index;
    next  = invalid_index;
    empty = true;
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

//clear helper MACROS
#undef proto
#undef proto_instance
#undef proto_header
} // namespace FlowGUI

#endif // FLOWGUI_NODETYPEILL_H
