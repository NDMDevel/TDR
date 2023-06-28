#include "Node.h"

//std

//Qt

//GuiBlocks
#include "GuiBlocks/Exceptions.h"

namespace GuiBlocks {

Node::Node()
{
    attr = Attributes::empty;
}

Node::~Node()
{
    releaseResource();
//    if( port != nullptr )
//        port->disconnectPortFromLink();
}

Node::Node(const QPointF &p, uint32_t id, uint32_t parent, Block::Port *bport)
    : point(p),
      parentIdx(parent),
      attr(Attributes::clear),
      linkId(id),
      port(bport)
{
}

Node::Node(Node &&n) noexcept
    : point(n.point),
      parentIdx(n.parentIdx),
      attr(n.attr),
      linkId(n.linkId),
      port(n.port)
{
    //not need to move (also moving to itself will
    //break the code because of "n.port = nullptr"
    //and the folowwing lines)
    if( this == &n )
        return;
    n.port = nullptr;
    n.attr = Attributes::empty;
}

Node &Node::operator=(const Node &n)
{
    point  = n.point;
    parentIdx = n.parentIdx;
    attr   = n.attr;
    linkId = n.linkId;
    port   = n.port;
    return *this;
}

Node &Node::operator=(Node &&n) noexcept
{
    //not need to move (also moving to itself will
    //break the code because of "n.port = nullptr"
    //and the folowwing lines)
    if( this == &n )
        return *this;
    point  = n.point;
    parentIdx = n.parentIdx;
    attr   = n.attr;
    linkId = n.linkId;
    port   = n.port;

    n.port = nullptr;
    n.attr = Attributes::empty;
    return *this;
}

GuiBlocks::Node::operator bool() const
{
    return !isEmpty();
}

bool Node::isEmpty() const
{
    return attr == Attributes::empty;
}

void Node::makeEmpty()
{
    attr = Attributes::empty;
    releaseResource();
}

uint32_t Node::getLinkId() const
{
    if( isEmpty() )
        GUIBLOCK_REPORT(NodeInvalidAccess());
    return linkId;
}

QPointF Node::getPoint() const
{
    return point;
}

void Node::setPoint(const QPointF &p)
{
    point = p;
}

void Node::releaseResource()
{
    if( port != nullptr )
    {
        port->disconnectPortFromLink();
        port = nullptr;
    }
}



} // namespace GuiBlocks
