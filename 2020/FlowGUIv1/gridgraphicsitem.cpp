#include "gridgraphicsitem.h"

#include <QPainter>
#include <QDebug>

GridGraphicsItem::GridGraphicsItem(const QRectF &boundRect,uint32_t gridSize)
    : gridSize(gridSize),
      boundRect(boundRect)
{
}

void GridGraphicsItem::updateBoundRect(const QRectF &boundRect)
{
    this->boundRect = boundRect;
    update();
}

void GridGraphicsItem::setPen(const QColor &color,float width)
{
    pen.setColor(color);
    pen.setWidthF(double(width));
}

void GridGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //just to avoid unsed vars warnings
    if( widget == nullptr || option == nullptr ){}

    painter->setPen(pen);

    qreal left = (boundRect.left()) - (uint32_t(boundRect.left()) % gridSize);
    qreal top = (boundRect.top()) - (uint32_t(boundRect.top()) % gridSize);
    QVarLengthArray<QPointF> points;
    for (qreal x = left; x < boundRect.right(); x += gridSize)
        for (qreal y = top; y < boundRect.bottom(); y += gridSize)
            points.append(QPointF(x,y));
    painter->drawPoints(points.data(), points.size());
}
