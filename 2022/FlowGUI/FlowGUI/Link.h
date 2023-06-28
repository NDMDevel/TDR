#ifndef FLOWGUI_LINK_H
#define FLOWGUI_LINK_H

//FlowGUI
#include "LinkImp/LinkImp.h"

//std
#include <cstdint>

//Qt
#include <QGraphicsItem>

namespace FlowGUI {
#ifndef LINKIMP_NOTEMPLATE
class Link : public QGraphicsItem, public LinkImp<QPointF,int,uint32_t>
#else
class Link : public QGraphicsItem, public LinkImp
#endif
{
public:
    Link(const QPointF& startPos = QPointF());

    //QGraphicsItem pure virtual methods
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    //helpers
    void updateBoundingRect();

private:
    QRectF _renderRect;
public:
    struct
    {

    }debug;
};

} // namespace FlowGUI

#endif // FLOWGUI_LINK_H
