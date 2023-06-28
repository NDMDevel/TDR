#ifndef CUSTOMRECT_H
#define CUSTOMRECT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

class CustomRect : public QGraphicsItem
{
private:
    QRectF boundRect;
    QPointF mouseStartPos;
    bool clickPressed;
    bool itemDrag;
    QPainterPath path;
    QPainterPath input;

public:
    CustomRect(QRectF boundRect = QRectF(0,0,100,100));
    QRectF boundingRect() const override { return boundRect; }
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) override;

    void addInput(int );

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
};

#endif // CUSTOMRECT_H
