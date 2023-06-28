//FlowGUI
#include "Block.h"
#include "Style.h"
#include "Utils.h"

//std

//qt
#include <QGraphicsItem>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

namespace FlowGUI {

Block::Block(const QString& typeName,
             const QString& instanceName,
             bool  selected,
             bool  sizeable)
    : _typeName(typeName)
    , _instanceName(instanceName)
    , _selected(selected)
    , _sizeable(sizeable)
{
    _ports.reserve(4);
    //QUuid::createUuid()
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    //block flags:
    //setFlags(QGraphicsItem::ItemIsMovable);
    //setFlags(QGraphicsItem::ItemIsSelectable);
    setAcceptDrops(true);
    setAcceptHoverEvents(true);

    //shadow effect:
    setBlockEffect(StyleBlockShape::shadowColor);

    //block style:
    setOpacity(StyleBlockShape::opacity);

    updateBoundingRect();
}

void Block::paint(QPainter *painter,
                  const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawShape(painter);
    drawTypeName(painter);
    drawInstanceName(painter);
    drawPorts(painter);

    //debug:
    painter->drawRect(_boundingRect);
//    painter->drawRect(_draggableRect);

//    painter->drawLine(_renderRect.topLeft(),_renderRect.bottomRight());
//    painter->drawLine(_renderRect.topRight(),_renderRect.bottomLeft());
}

void Block::addPort(PortType&&  port,
                    Block::Face face,
                    PortDir     dir,
                    uint32_t    idx,
                    uint32_t    layer,
                    bool        hide_)
{
    if( idx == invalid_index )
        idx = nextPortIdx(face);
    insertPort(std::move(port),face,dir,idx,layer,hide_);

    _shapeChanged = true;
    //first the _boundingRect will expanded so
    //all the ports have enough space. After this
    //the _boundingRect will be able to hold all
    //the ports and texts related to the type and
    //instance of the block
    updateBoundingRect();

    //now update the ports's QPainterPath accordingly
    //so when drawPorts() is invoked from paint() the
    //ports are located at the right position
    updatePorts();
    _shapeChanged = false;

    //now repaint the block
    update();
}
Block::port_t *Block::portUnderMouse(const QPointF &pos)
{
    qDebug() << "portUnderMouse" << mapFromScene(pos);
    if( _ports.size() == 0 )
        return nullptr;
    for( auto& port : _ports )
        if( port.shape.contains(mapFromScene(pos)) )
        {
            qDebug() << "(port found)";
            return &port;
        }
    return nullptr;
}
bool Block::mouseOverBlock(const QPointF &pos) const
{
    return _draggableRect.contains(pos);
}
void Block::rotate(bool clockWise)
{
    for( auto& port : _ports )
    {
        if( clockWise )
            switch( port.face )
            {
            case Face::Left:
                port.face = Face::Top;
                break;
            case Face::Top:
                port.face = Face::Right;
                break;
            case Face::Right:
                port.face = Face::Bottom;
                break;
            case Face::Bottom:
                port.face = Face::Left;
                break;
            }
        else
            switch( port.face )
            {
            case Face::Left:
                port.face = Face::Bottom;
                break;
            case Face::Bottom:
                port.face = Face::Right;
                break;
            case Face::Right:
                port.face = Face::Top;
                break;
            case Face::Top:
                port.face = Face::Left;
                break;
            }
    }
    _shapeChanged = true;
    updateBoundingRect();
    updatePorts();
    _shapeChanged = false;
    update();
}
void Block::flip(bool vertical)
{
    for( size_t idx=0 ; idx<_ports.size()/2 ; ++idx )
        std::swap(_ports[idx],_ports[_ports.size()-1-idx]);

    if( vertical )
    {
        for( auto& port : _ports )
        {
            if( port.face == Face::Top )
            {
                port.face = Face::Bottom;
                continue;
            }
            if( port.face == Face::Bottom )
            {
                port.face = Face::Top;
                continue;
            }
        }
    }
    else
    {
        for( auto& port : _ports )
        {
            if( port.face == Face::Left )
            {
                port.face = Face::Right;
                continue;
            }
            if( port.face == Face::Right )
            {
                port.face = Face::Left;
                continue;
            }
        }
    }
    _shapeChanged = true;
//    updateBoundingRect();
    updatePorts();
    _shapeChanged = false;
    update();
}
void Block::select()
{
    _selected = true;
    update();
}
void Block::deselect()
{
    _selected = false;
    update();
}
void Block::showPorts() const
{
    for( uint32_t idx=0 ; idx<_ports.size() ; idx++ )
    {
        qDebug() << "[" << idx << "]: " << _ports[idx].toString();
    }
}
void Block::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    bool needsUpdate = false;
    if( _draggableRect.contains(mapToItem(this,event->pos())) )
    {
        if( !_hover )
        {
            _hover = true;
            needsUpdate = true;
        }
    }
    else
    {
        if( _hover )
        {
            _hover = false;
            needsUpdate = true;
        }
    }
    for( auto& port : _ports )
        if( port.shape.contains(mapToItem(this,event->pos())) )
        {
            if( port.hover == false )
            {
                port.hover = true;
                needsUpdate = true;
            }
        }
        else
        {
            if( port.hover == true )
            {
                port.hover = false;
                needsUpdate = true;
            }
        }
    if( needsUpdate )
        update();
#warning "Remove this, valid only for debuging"
//    if( _portShape.translated(_boundingRect.left(),0.0).contains(mapToItem(this,event->pos())) )
//    {
//        if( !_hoverPort )
//        {
//            _hoverPort = true;
//            update();
//        }
//    }
//    else
//    {
//        if( _hoverPort )
//        {
//            _hoverPort = false;
//            update();
//        }
//    }
}
void Block::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    auto needsUpdate = false;
    if( _hover )
    {
        _hover = false;
        needsUpdate = true;
    }
    for( auto& port : _ports )
    {
        if( port.hover )
        {
            port.hover = false;
            needsUpdate = true;
        }
    }
    if( needsUpdate )
        update();

//    if( _hoverPort )
//    {
//        _hoverPort = false;
//        update();
//    }
}
void Block::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if( _dragging )
    {
        auto endPos   = nearestGridPosition(mapToScene(event->pos()),StyleGrid::gridSize);
        moveBy(endPos.x()-_startDrag.x(),
               endPos.y()-_startDrag.y());
        _startDrag = endPos;
    }
    else
        QGraphicsItem::mouseMoveEvent(event);
}
void Block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    _startDrag = nearestGridPosition(mapToScene(event->pos()),StyleGrid::gridSize);
    if( _draggableRect.contains(nearestGridPosition(mapToItem(this,event->pos()),StyleGrid::gridSize)) )
    {
        _dragging = true;
//        _selected = true;
    }
    else
        QGraphicsItem::mousePressEvent(event);
}
void Block::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    _dragging = false;
    QGraphicsItem::mouseReleaseEvent(event);
}
void Block::drawShape(QPainter *painter) const
{
    painter->save();

    QLinearGradient gradient(_draggableRect.width() *StyleBlockShape::customFillGradient.start().x(),
                             _draggableRect.height()*StyleBlockShape::customFillGradient.start().y(),
                             _draggableRect.width() *StyleBlockShape::customFillGradient.finalStop().x(),
                             _draggableRect.height()*StyleBlockShape::customFillGradient.finalStop().y());

    auto color = QColor(StyleBlockShape::blockRectFillColor1);
    color.setAlphaF(StyleBlockShape::fillOpacity);
    gradient.setColorAt(0.0,color);
    color = QColor(StyleBlockShape::blockRectFillColor2);
    color.setAlphaF(StyleBlockShape::fillOpacity);
    gradient.setColorAt(0.7,color);

    painter->setBrush(gradient);
    if( _hover )
        painter->setPen(QPen(StyleBlockShape::blockRectBorderColorOnHover,2));
    else if( _selected )
        painter->setPen(QPen(StyleBlockShape::blockRectBorderColorIfSelected,1));
    else
        painter->setPen(QPen(StyleBlockShape::blockRectBorderColor,1));

    painter->drawPath(_blockShape);

    painter->restore();
}
void Block::drawPorts(QPainter *painter) const
{
    if( _ports.size() == 0 )
        return;
    painter->save();
//    auto path = _portShape.translated(_boundingRect.left(),StyleGrid::gridSize);
    for( const auto& port : _ports )
    {
        if( port.hide )
            continue;
        painter->save();
        auto path = port.shape;//.translated(_draggableRect.left()-StyleBlockShape::connectorSizeGridSizePercent.width()*StyleGrid::gridSize,0);
        if( port.hover )
        {
            painter->setPen(Qt::magenta);
            painter->setBrush(Qt::magenta);
            path.closeSubpath();
        }
        painter->drawPath(path);
        painter->restore();
    }
    painter->restore();
}
void Block::drawTypeName(QPainter *painter) const
{
    if( _typeName.isEmpty() )
        return;

    painter->save();

    painter->setPen(StyleText::blockTypeColor);
    painter->setFont(StyleText::blockTypeFont);
    QFontMetrics fontMetrics = QFontMetrics(StyleText::blockTypeFont);
    QRectF textRect = fontMetrics.boundingRect(_typeName);
    QPointF offsetPos = textRect.center();
    if( !_instanceName.isEmpty()  )
        offsetPos.setY(offsetPos.y()+textRect.height()/1.75);
    painter->drawText(_draggableRect.center()-offsetPos,_typeName);

    painter->restore();
}
void Block::drawInstanceName(QPainter *painter) const
{
    if( _instanceName.isEmpty() )
        return;

    painter->save();

    painter->setPen(StyleText::blockNameColor);
    painter->setFont(StyleText::blockNameFont);
    QFontMetrics fontMetrics = QFontMetrics(StyleText::blockNameFont);
    QRectF textRect = fontMetrics.boundingRect(_instanceName);
    QPointF offsetPos = textRect.center();
    if( !_typeName.isEmpty() )
        offsetPos.setY(offsetPos.y()-textRect.height()/1.75);
    painter->drawText(_draggableRect.center()-offsetPos,_instanceName);

    painter->restore();
}
void Block::updateBoundingRect()
{
    if( !_shapeChanged )
        return;

    auto hvPortSize = [&]()
    {
        uint32_t lidx = 0;
        uint32_t bidx = 0;
        uint32_t ridx = 0;
        uint32_t tidx = 0;
        for( const auto& port : _ports )
        {
            switch( port.face )
            {
            case Face::Left:
                lidx = std::max(lidx,port.idx);
                break;
            case Face::Bottom:
                bidx = std::max(bidx,port.idx);
                break;
            case Face::Right:
                ridx = std::max(ridx,port.idx);
                break;
            case Face::Top:
                tidx = std::max(tidx,port.idx);
                break;
            }
        }
        uint32_t vsize;
        uint32_t hsize;
        vsize = std::max(lidx,ridx);
        hsize = std::max(bidx,tidx);
        if( vsize != 0 )
            vsize++;
        if( hsize != 0 )
            hsize++;
        return std::tuple<double,double>(double(hsize)*StyleGrid::gridSize,
                                         double(vsize)*StyleGrid::gridSize);
    };
    auto hvTextSize = [&]()
    {
        //compute inner block width (enough space to write the _typeName of the block plus some gap)
        QFontMetrics fontMetrics = QFontMetrics(StyleText::blockTypeFont);
        double typeTextWidth  =   2.0*StyleText::gapTypeToBorderGridSizePercent*fontMetrics.capHeight()
                                + fontMetrics.horizontalAdvance(_typeName);
        double typeTextHeight = fontMetrics.capHeight();
        fontMetrics = QFontMetrics(StyleText::blockNameFont);
        double instTextWidth  =   2.0*StyleText::gapTypeToBorderGridSizePercent*fontMetrics.capHeight()
                                + fontMetrics.horizontalAdvance(_instanceName);
        double instTextHeight = fontMetrics.capHeight();

        auto textWidth  = std::max(typeTextWidth,instTextWidth);
        auto textHeight = typeTextHeight + instTextHeight;
//        textWidth  = nextGridValue(textWidth,StyleGrid::gridSize);
//        textHeight = nextGridValue(textHeight,StyleGrid::gridSize);

        return std::tuple<double,double>(textWidth,textHeight);
    };
    auto hvMax = [&](const std::tuple<double,double>& arg1,
                     const std::tuple<double,double>& arg2)
    {
        auto width  = std::max(std::get<0>(arg1),std::get<0>(arg2));
        auto height = std::max(std::get<1>(arg1),std::get<1>(arg2));
        return std::tuple<double,double>(width,height);
    };

    auto[hsize,vsize] = hvMax(hvPortSize(),hvTextSize());
    _draggableRect = QRectF(-hsize/2.0,-vsize/2.0,hsize,vsize);
    auto center = nearestGridPosition(_draggableRect.center(),StyleGrid::gridSize);
    center.setX(center.x() - StyleGrid::gridSize/2.0);
    center.setY(center.y() + StyleGrid::gridSize/2.0);
    auto left   = _draggableRect.left();
    auto right  = _draggableRect.right();
    auto bottom = _draggableRect.bottom();
    auto top    = _draggableRect.top();
    const auto portWidth = (StyleBlockShape::connectorSizeGridSizePercent*StyleGrid::gridSize).width();
    left   = nextGridValue(left   - portWidth,StyleGrid::gridSize,false)+portWidth;
    right  = nextGridValue(right  + portWidth,StyleGrid::gridSize,true )-portWidth+StyleGrid::gridSize;
    top    = nextGridValue(top    - portWidth,StyleGrid::gridSize,false)+portWidth-StyleGrid::gridSize;
    bottom = nextGridValue(bottom + portWidth,StyleGrid::gridSize,true)-portWidth;
    hsize  = right  - left;
    vsize  = bottom - top;
    _draggableRect.setLeft(left);
    _draggableRect.setBottom(bottom);
    _draggableRect.setRight(right);
    _draggableRect.setTop(top);

    _boundingRect  = QRectF(0,0,hsize+2.0*portWidth,vsize+2.0*portWidth);
    _boundingRect.moveCenter(_draggableRect.center());

    _blockShape = QPainterPath();
    switch( 0 )
    {
    case 0:
        _blockShape.addRoundedRect(_draggableRect,
                                   _draggableRect.width() *StyleBlockShape::roundingXWidthPercent,
                                   _draggableRect.height()*StyleBlockShape::roundingYWidthPercent);
        break;
    case 1:
        _blockShape.addEllipse(_draggableRect);
        break;
    }
}
void Block::setBlockEffect(const QColor &color)
{
    auto effect = new QGraphicsDropShadowEffect;
    //effect->boundingRectFor(dragArea);
    effect->setOffset(2, 2);
    effect->setBlurRadius(15);
    effect->setColor(color);
    setGraphicsEffect(effect);
}
uint32_t Block::nextPortIdx(const Block::Face &face) const
{
    uint32_t lastIdx = 0;
    for( uint32_t idx=0 ; idx<_ports.size() ; ++idx )
        if( _ports[idx].face == face )
            lastIdx = std::max(lastIdx,_ports[idx].idx+1);

    return lastIdx;
}
void Block::insertPort(PortType&&  port,
                       Block::Face face,
                       PortDir     dir,
                       uint32_t    idx,
                       uint32_t    layer,
                       bool        hide_)
{
    bool displaceUp = false;
    for( const auto& port : _ports )
        if( port.idx == idx && port.face == face )
        {
            displaceUp = true;
            break;
        }
    if( displaceUp )
        std::for_each(_ports.begin(),_ports.end(),[&](port_t& port)
        {
            if( (port.face == face) && (port.idx >= idx) )
                port.idx++;
        });
    _ports.emplace_back(port_t(std::move(port),
                               face,
                               dir,
                               idx,
                               hide_,
                               layer,
                               PortShape::Triangular));
    std::sort(_ports.begin(),_ports.end(),[](const port_t& p1,const port_t& p2)
    {
        if( p1.face == p2.face )
            return p1.idx < p2.idx;
        return p1.face < p2.face;
    });
}
void Block::updatePorts()
{
    if( _ports.size() == 0 )
        return;
    auto getConnOffset = [&](Face face,uint32_t idx)
    {
        switch( face )
        {
        case Face::Left:
            {
                auto left = _draggableRect.left();
                auto top  = _draggableRect.top();
                left = nextGridValue(left,StyleGrid::gridSize,false);
                top  = nextGridValue(top ,StyleGrid::gridSize,true );
                top += StyleGrid::gridSize*idx;
                return QPointF(left,top+StyleGrid::gridSize);
            }
            break;
        case Face::Bottom:
            {
                auto left   = _draggableRect.left();
                auto bottom = _draggableRect.bottom();
                left    = nextGridValue(left,StyleGrid::gridSize);
                bottom  = nextGridValue(bottom ,StyleGrid::gridSize);
                left   += StyleGrid::gridSize*idx;
                return QPointF(left+StyleGrid::gridSize,bottom);
            }
            break;
        case Face::Right:
            {
                auto right  = _draggableRect.right();
                auto bottom = _draggableRect.bottom();
                right   = nextGridValue(right,StyleGrid::gridSize);
                bottom  = nextGridValue(bottom ,StyleGrid::gridSize,false);
                bottom -= StyleGrid::gridSize*idx;
                return QPointF(right,bottom-StyleGrid::gridSize);
            }
            break;
        case Face::Top:
            {
                auto right = _draggableRect.right();
                auto top   = _draggableRect.top();
                right = nextGridValue(right,StyleGrid::gridSize,false);
                top   = nextGridValue(top ,StyleGrid::gridSize,false);
                right  -= StyleGrid::gridSize*idx;
                return QPointF(right-StyleGrid::gridSize,top);
            }
            break;
        }
    };
    for( auto& port : _ports )
        {
            port.resetPortShape(port.portShape);
            port.shape.translate(getConnOffset(port.face,port.idx));
        }
}


} // namespace FlowGUI
