#ifndef BLOCKVIEW_H
#define BLOCKVIEW_H

#include <QGraphicsItem>
#include <QPainter>

#include "bindey/property.h"
#include "bindey/binding.h"
#include "blockviewmodel.h"

class BlockView : public QGraphicsItem
{
public: //QGrapchisItem methods
    BlockView(BlockViewModel& vm,QGraphicsItem* parent = nullptr);
    ~BlockView();
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

public: //general
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
public: //property
//    bindey::property<QSize>     _size;

private:
//    std::vector<bindey::binding> _bonds;
    BlockViewModel& _vm;
};

#endif // BLOCKVIEW_H
