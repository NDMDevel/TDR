#include "blockview.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

BlockView::BlockView(BlockViewModel& vm,QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , _vm(vm)
{
    //QGraphicsItem config
    setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsMovable);
    setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable);
    setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsFocusable);
    setAcceptHoverEvents(true);
//    _size.set(QSize(50,50));

    //Data binding:
    //view -> view model
//    _bonds.push_back( bindey::bind(_vm._size,_size) );
//    bindey::bind(_size,_vm._size);
//    _size.set(_vm._size.get());
    _vm.changedSignal.connect([&](){update();});
}
BlockView::~BlockView()
{
    //disconnect all links to the view model
//    for( auto& bond : _bonds )
//        bond.disconnect();
}
QRectF BlockView::boundingRect() const
{
//    auto size = _size.get();
    auto size = _vm._size.get();
    return QRectF(-size.width()/2,
                  -size.height()/2,
                  size.width(),
                  size.height());
}
void BlockView::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
//    auto size = _size.get();
    auto size = _vm._size.get();
    painter->drawRoundedRect(-size.width()/2+1,-size.height()/2+1,
                             size.width()-2,size.height()-2,
                             5,5);
    if( _vm._drawMark.get() )
        painter->drawEllipse(_vm._markPoint.get().x()-5,
                             _vm._markPoint.get().y()-5,
                             10,
                             10);
}
void BlockView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    BlockViewModel::MouseEventMetadata mdata;
    if( event->button() & Qt::RightButton )
        mdata.right = true;
    if( event->button() & Qt::LeftButton )
        mdata.left = true;
    if( event->button() & Qt::MiddleButton )
        mdata.middle = true;
    if( event->modifiers() & Qt::ControlModifier )
        mdata.ctrl = true;
    if( event->modifiers() & Qt::ShiftModifier )
        mdata.shift = true;
    if( event->modifiers() & Qt::AltModifier )
        mdata.alt = true;

    _vm.onClick(QPoint(event->pos().x(),event->pos().y()),mdata);
}
void BlockView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverMoveEvent(event);
    BlockViewModel::MouseEventMetadata mdata;
    if( event->modifiers() & Qt::KeyboardModifier::ControlModifier )
        mdata.ctrl = true;
    if( event->modifiers() & Qt::KeyboardModifier::ShiftModifier )
        mdata.shift = true;
    if( event->modifiers() & Qt::KeyboardModifier::AltModifier )
        mdata.alt = true;
    _vm.onMouseMove(QPoint(event->pos().x(),event->pos().y()),mdata);
}
