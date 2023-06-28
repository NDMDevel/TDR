//FlowGUI
#include "LinkImp/LinkTreeArray.h"
#include "Exceptions.h"

//std

//Qt


namespace FlowGUI {

LinkTreeArray::LinkTreeArray()
    : nodes(3)
    , size(0)
{}

Idx LinkTreeArray::insert(const Coord &coord) noexcept
{
    return createNode(node_t(coord));
}

Idx LinkTreeArray::append(Idx parentIdx, const Coord &coord) noexcept
{
    if( parentIdx >= size )
    {
        FLOWGUI_REPORT(IndexOutOfRange());
        return invalid_index;
    }
    if( nodes[parentIdx].isEmpty() )
    {
        FLOWGUI_REPORT(IndexOutOfRange());
        return invalid_index;
    }
    //parentIdx is a valid index, now do the append
    //creates a new node (uses one that is empty or
    //alloc a new one). The created node will be not
    //empty. The index where is located is returned.

    auto idx = createNode(node_t(coord,parentIdx));
    if( idx != invalid_index )
        nodes[parentIdx].next = idx;
    return idx;
}

Idx LinkTreeArray::insertBefore(Idx targetIdx, const Coord &coord) noexcept
{

}

void LinkTreeArray::removeSubTree(Idx targetIdx) noexcept
{

}

std::optional<Coord> LinkTreeArray::operator[](Idx targetIdx) noexcept
{
    return {};
}

void LinkTreeArray::resetIter(LinkContainerInterface::Iter &iter) noexcept
{
    if( size == 0 )
    {
        iter.from = invalid_index;
        iter.to   = invalid_index;
        return;
    }
    for( Idx from=0 ; from<nodes.size() ; from++ )
        if( !nodes[from].isEmpty() )
        {
            if( auto to = nodes[from].next ; to < nodes.size() && !nodes[to].isEmpty() )
            {
                iter.from = from;
                iter.to   = to;
                return;
            }
            else
            {
                iter.from = invalid_index;
                iter.to   = invalid_index;
                return;
            }
        }
}

std::optional<Coord>
    LinkTreeArray::iterate(Iter &iter) const noexcept
{
    if( iter.from == invalid_index )
        return {};
    const auto localIter = iter;
    iter.from = iter.to;
    if( iter.to != invalid_index )
    {
        if( const auto &node = nodes[iter.to]; !node.isEmpty() )
            iter.to   = node.next;
        else
            iter.to   = invalid_index;
    }
    return {nodes[localIter.from].coord};
}

std::optional<std::tuple<Coord,Coord>>
    LinkTreeArray::iterateLine(Iter &iter) const noexcept
{
    if( auto idx = iterateLineIdx(iter) )
    {
        const auto[from,to] = idx.value();
        return {{nodes[from].coord,nodes[to].coord}};
    }
    return {};
}

std::optional<std::tuple<const Idx&,const Idx&>>
    LinkTreeArray::iterateLineIdx(Iter &iter) const noexcept
{
    if( iter.to == invalid_index || iter.from == invalid_index )
        return {};
    const auto localIter = iter;
    if( const auto &node = nodes[iter.to]; !node.isEmpty() )
    {
        iter.from = iter.to;
        iter.to   = node.next;
    }
    else
    {
        iter.from = invalid_index;
        iter.to   = invalid_index;
    }
    return {{localIter.from,localIter.to}};
}

Idx LinkTreeArray::createNode(node_t &&node) noexcept
{
    for( Idx i=0 ; i<nodes.size() ; i++ )
        if( nodes[i].isEmpty() )
        {
            size++;
            nodes[i] = std::move(node);
            return i;
        }
    nodes.emplace_back(std::move(node));
    size++;
    return nodes.size()-1;
}

} // namespace FlowGUI
