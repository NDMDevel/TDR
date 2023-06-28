#ifndef GRIDGRAPHICSITEM_H
#define GRIDGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QPen>

class GridGraphicsItem : public QGraphicsItem
{
private:
    uint32_t gridSize;
    QRectF boundRect;
    QPen pen;
public:
    GridGraphicsItem(const QRectF &boundRect,uint32_t gridSize);
    void updateBoundRect(const QRectF &boundRect);

    void setPen(const QColor &color,float width = 1.5);
    QRectF boundingRect() const override { return boundRect; }
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
protected slots:

//    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

};

#endif // GRIDGRAPHICSITEM_H
