#include "ViewBlock.h"
#include <QPainter>
#include "binding.h"

ViewBlock::ViewBlock(ViewModelBlock& viewModel)
    : _viewModel(viewModel)
{

}
QRectF ViewBlock::boundingRect() const
{
    return {-50,-50,100,100};
}
void ViewBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void)option;
    (void)widget;

    painter->drawRect(boundingRect());
    painter->drawText(QPointF(-45,+25),name);
    painter->drawText(QPointF(-45,-25),type);
}
