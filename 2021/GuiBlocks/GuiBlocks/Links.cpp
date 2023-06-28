#include "Links.h"

//std

//Qt

//GuiBlocks
#include "GuiBlocks/Utils.h"
#include "GuiBlocks/Exceptions.h"

namespace GuiBlocks {

//-------------------------------------
// Public Interface:
Links::Links()
    : startIdx(idx32::invalid_index),
      midIdx(idx32::invalid_index),
      endIdx(idx32::invalid_index),
      _boundingRect(QRectF()),
      _size(0)
{
    nodes.reserve(3);
}

void Links::appendRandomLine()
{
    startLineAt(QPointF(),QPointF(100,200),LinkPath::straightThenOrthogonal);
    updateBoundingRect();
}

void Links::paint(QPainter *painter,
                  const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    (void)option;
    (void)widget;

    painter->drawRect(_boundingRect*0.9);
    auto iter = resetIter();
    while( const auto& indexs = iterate(iter) )
    {
        auto[from,to] = indexs.value();
        //draw line:
        painter->drawLine(nodes[from].getPoint(),nodes[to].getPoint());
        //draw node:
        #warning "implement the attribute 'is node' when the node is parent of multiple childrens"
        //if( tree.childrenCount(to) > 1 )
        //painter->drawEllipse(nodes[to].getPoint(),2,2);
    }
}

QRectF Links::boundingRect() const
{
    return _boundingRect;
}

// Line Management Methods:
void Links::startLineAt(const QPointF &start,
                        const QPointF &end,
                        Links::LinkPath path,
                        Block::Port *connPort)
{
    auto parentIdx = idx32::invalid_index;
    auto linkId = getUnusedLinkId();
    startIdx = appendNode(Node(start,linkId,parentIdx,connPort));
    if( auto midPoint = computeMidPoint(start,end,path) )
        midIdx = appendNode(Node(midPoint.value(),linkId));
    endIdx = appendNode(Node(end,linkId));

    updateBoundingRect();
    prepareGeometryChange();
    update();
}

void Links::updateLastLine(const QPointF &end, Links::LinkPath path)
{
    if( size() < 2 || startIdx == idx32::invalid_index )
    {
        GUIBLOCK_REPORT(LinkWithNoLastLine());
        return;
    }
    if( const auto& midp = computeMidPoint(nodes[startIdx].getPoint(),end,path) )
    {
        //insert midp or update if exists
        if( midIdx != idx32::invalid_index )
            //update
            nodes[midIdx].setPoint(midp.value());
        else
            //insert
            midIdx = appendNode(Node(midp.value(),nodes[startIdx].getLinkId(),startIdx));
        nodes[endIdx].setPoint(end);
    }
    else
    {
        //remove midp if exists, or update end
        if( midIdx != idx32::invalid_index )
        {
            //tree.removeSubTree(endIdx);
            nodes[midIdx] = std::move(nodes[endIdx]);
            nodes[midIdx].setPoint(end);
            nodes[endIdx].makeEmpty();
            endIdx = midIdx;
            midIdx = idx32::invalid_index;
        }
        else
            nodes[endIdx].setPoint(end);
    }
    updateBoundingRect();
    prepareGeometryChange();
    update();
}

void Links::removeLastLine()
{
    if( size() < 2 || startIdx == idx32::invalid_index )
    {
        GUIBLOCK_REPORT(LinkWithNoLastLine());
        return;
    }
}

uint32_t Links::size() const
{
    return _size;
}

void Links::resetIter(Links::IterHandler &iter) const noexcept
{
    if( _size < 2 )
    {
        iter.from = idx32::invalid_index;
        iter.to   = idx32::invalid_index;
        return;
    }
    iter.from = getNextNonEmptyNode();
    iter.to   = getNextNonEmptyNode(iter.from);
    if( iter.to == idx32::invalid_index )
        iter.from = idx32::invalid_index;
}

Links::IterHandler Links::resetIter() const noexcept
{
    IterHandler iter;
    resetIter(iter);
    return iter;
}

std::optional<std::tuple<uint32_t,uint32_t>>
Links::iterate(Links::IterHandler &iter) const noexcept
{
    if( iter.from == idx32::invalid_index || iter.to == idx32::invalid_index )
        return {};

    auto from = iter.from;
    auto to   = iter.to;
    if( iter.to = getNextNonEmptyNode(iter.to); iter.to == idx32::invalid_index )
    {
        iter.to = getNextNonEmptyNode(iter.to);
        if( iter.from = getNextNonEmptyNode(iter.from); iter.from == idx32::invalid_index )
            iter.to = idx32::invalid_index;
    }

    return {{from,to}};
}

//-------------------------------------
// Internal Methdos:
uint32_t Links::getUnusedLinkId() const
{
    uint32_t id = 0;
    while( true )
    {
        repeat:
        for( const auto& node : nodes )
        {
            if( node.isEmpty() )
                continue;
            if( id == node.getLinkId() )
            {
                id++;
                goto repeat;
            }
        }
        return id;
    }
}

void Links::updateBoundingRect()
{
    if( size() < 2 )
    {
        GUIBLOCK_REPORT(LinkIsNowEmpty());
        _boundingRect = QRectF();
        return;
    }
    bool first = true;
    auto ptl = QPointF();
    auto pbr = QPointF();
    for( const auto& node : nodes )
    {
        if( node.isEmpty() )
            continue;
        if( first )
        {
            first = false;
            ptl = node.getPoint();
            pbr = node.getPoint();
        }
        auto x = node.getPoint().x();
        auto y = node.getPoint().y();
        if( ptl.x() > x )
            ptl.setX(x);
        if( ptl.y() < y )
            ptl.setY(y);
        if( pbr.x() < x )
            pbr.setX(x);
        if( pbr.y() > y )
            pbr.setY(y);
    }
    ptl.setX(ptl.x()-double(StyleGrid::gridSize));
    ptl.setY(ptl.y()+double(StyleGrid::gridSize));
    pbr.setX(pbr.x()+double(StyleGrid::gridSize));
    pbr.setY(pbr.y()-double(StyleGrid::gridSize));
    _boundingRect = QRectF(ptl,pbr);
}

std::optional<QPointF> Links::computeMidPoint(const QPointF &startPoint, const QPointF &endPoint, const Links::LinkPath &linkPath) const noexcept
{
    QPointF midp;
    switch( linkPath )
    {
        case LinkPath::straight:
            return {};
        case LinkPath::verticalThenHorizontal:
            midp = QPointF( startPoint.x() , endPoint.y() );
            break;
        case LinkPath::horizontalThenVertical:
            midp = QPointF( endPoint.x() , startPoint.y() );
            break;
        case LinkPath::straightThenOrthogonal:
            {
                auto diffp = startPoint-endPoint;
                auto diffx = std::abs(diffp.x());
                auto diffy = std::abs(diffp.y());
                if( diffx < diffy )
                {
                    if( endPoint.y() > startPoint.y() )
                        midp = QPointF( endPoint.x() , startPoint.y()+diffx);
                    else
                        midp = QPointF( endPoint.x() , startPoint.y()-diffx);
                }
                else
                {
                    if( endPoint.x() > startPoint.x() )
                        midp = QPointF( startPoint.x()+diffy , endPoint.y() );
                    else
                        midp = QPointF( startPoint.x()-diffy , endPoint.y() );
                }
            }
            break;
        case LinkPath::orthogonalThenStraight:
            {
                auto diffp = startPoint-endPoint;
                auto diffx = std::abs(diffp.x());
                auto diffy = std::abs(diffp.y());
                if( diffx <= diffy )
                {
                    if( endPoint.y() < startPoint.y() )
                        midp = QPointF( startPoint.x() , endPoint.y()+diffx);
                    else
                        midp = QPointF( startPoint.x() , endPoint.y()-diffx);
                }
                else
                {
                    if( endPoint.x() < startPoint.x() )
                        midp = QPointF( endPoint.x()+diffy , startPoint.y() );
                    else
                        midp = QPointF( endPoint.x()-diffy , startPoint.y() );
                }
            }
            break;
    }
    return midp;
}

uint32_t Links::appendNode(Node &&n) noexcept
{
    _size++;
    auto targetIdx = getNextEmptyNode();
    if( targetIdx == idx32::invalid_index )
    {
        //the container must be expanded
        nodes.emplace_back(std::move(n));
        return nodes.size()-1;
    }
    //use an already existing empty node of the container
    nodes[targetIdx] = std::move(n);
    return targetIdx;
}

uint32_t Links::getNextEmptyNode(uint32_t startPos) const noexcept
{
    for( startPos=0 ; startPos < nodes.size() ; startPos++ )
        if( nodes[startPos].isEmpty() )
            return startPos;
    return idx32::invalid_index;
}

uint32_t Links::getNextNonEmptyNode(uint32_t startPos) const noexcept
{
    for( startPos = startPos+1 ; startPos<nodes.size() ; startPos++ )
        if( !nodes[startPos].isEmpty() )
            return startPos;
    return idx32::invalid_index;
}


} // namespace GuiBlocks
