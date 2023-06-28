#include "Links.h"

namespace GuiBlocks {

Links::Links()
{

}

void Links::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void)painter;
    (void)option;
    (void)widget;
}

QRectF Links::boundingRect() const
{
    return QRectF();
}

} // namespace GuiBlocks
