//FlowGUI
#include "Link.h"
#include "Exceptions.h"
#include "Style.h"

//std

//Qt
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>

namespace FlowGUI {

Link::Link(const QPointF& startPos)
    : LinkImp(startPos)
{
    //moveTo(startPos);
}
void Link::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    (void)option;
    (void)widget;

    painter->setPen(QPen(QBrush(Qt::black),
                         qreal(2),
                         Qt::DashLine,
                         StyleLink::normalCap));
    painter->drawRect(boundingRect());


    painter->setPen(QPen(QBrush(StyleLink::lineColor),
                         qreal(StyleLink::width),
                         StyleLink::normalLine,
                         StyleLink::normalCap));
    painter->setFont(StyleText::blockHintFont);

    PairIter iter(*this);
    while( const auto& coords = iterateNodes(iter) )
    {
        auto[p1,p2] = coords.value();
        painter->drawLine(p1.coord,p2.coord);

        //draw node:
        painter->save();
        painter->setPen(QPen(QBrush(StyleLink::nodeColor),3));
        if( p1.status.childs > 1 )
            painter->drawEllipse(p1.coord,2,2);
        if( p2.status.childs > 1 )
            painter->drawEllipse(p2.coord,2,2);
        painter->restore();
    }
}
QRectF Link::boundingRect() const
{
            return _renderRect;
}
void Link::updateBoundingRect()
{
    if( size() < 2 )
    {
        _renderRect = QRectF();
        prepareGeometryChange();
        return;
    }

    NodeIter iter(*this);
    if( auto coord = iterateCoords(iter) )
    {
        auto ptl = coord.value();
        auto pbr = ptl;
        while( const auto &node = iterateCoords(iter) )
        {
            const auto &coord = node.value();
            auto x = coord.x();
            auto y = coord.y();
            if( ptl.x() > x )
                ptl.setX(x);
            if( ptl.y() > y )
                ptl.setY(y);
            if( pbr.x() < x )
                pbr.setX(x);
            if( pbr.y() < y )
                pbr.setY(y);
        }
        ptl.setX(ptl.x()-double(StyleGrid::gridSize));
        ptl.setY(ptl.y()-double(StyleGrid::gridSize));
        pbr.setX(pbr.x()+double(StyleGrid::gridSize));
        pbr.setY(pbr.y()+double(StyleGrid::gridSize));
        _renderRect.setTopLeft(ptl);
        _renderRect.setBottomRight(pbr);
        prepareGeometryChange();
        return;
    }
    //this should be unreachable code
    FLOWGUI_REPORT(CodeUnreachable());
    _renderRect = QRectF();
}

} // namespace FlowGUI
