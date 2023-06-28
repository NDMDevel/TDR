#ifndef FLOWGUI_BLOCK_H
#define FLOWGUI_BLOCK_H

//FlowGUI
#include "BlockImp/PortType.h"
#include "Style.h"

//std
#include <vector>

//qt
#include <QGraphicsItem>
#include <QPainterPath>

namespace FlowGUI {

class Block : public QGraphicsItem
{
public:     //exported types
    enum class Face : uint8_t { Left, Bottom, Right, Top };
    enum class PortDir   : uint8_t { Input , Output };
    enum class PortShape : uint8_t { Triangular };
    struct port_t
    {
        port_t(PortType &&port,Face face,PortDir dir,uint32_t idx,
               bool hide,uint32_t layer,PortShape portShape)
            : port(std::move(port)),face(face),dir(dir),idx(idx)
            , hide(hide),layer(layer),portShape(portShape)
        {
            resetPortShape(portShape);
        }
        PortType port;
        Face     face;
        PortDir  dir;
        uint32_t idx;
        bool     hide  = false;
        uint32_t layer = 0;
        PortShape portShape = PortShape::Triangular;
        bool     hover = false;
        QPainterPath shape;
        void resetPortShape(PortShape portShape)
        {
            enum class PortShapeDir : uint8_t
            {
                TriConnLeft,
                TriConnRight,
                TriConnUp,
                TriConnDown
            };
            auto makeShape = [&](PortShapeDir shapeDir,PortDir dir)
            {
                shape = QPainterPath();
                auto size = StyleBlockShape::connectorSizeGridSizePercent*StyleGrid::gridSize;
                switch( shapeDir )
                {
                case PortShapeDir::TriConnLeft:
                    if( dir == PortDir::Input )
                    {
                        auto height = size.height();
                        auto width  = size.width();
                        shape.moveTo(0.0,-height/2.0);
                        shape.lineTo(width,0.0);
                        shape.lineTo(0.0, height/2.0);
                    }
                    if( dir == PortDir::Output )
                    {
                        auto height = size.height();
                        auto width  = size.width();
                        shape.moveTo(width,-height/2.0);
                        shape.lineTo(0.0,0.0);
                        shape.lineTo(width, height/2.0);
                    }
                    break;
                case PortShapeDir::TriConnRight:
                    if( dir == PortDir::Input )
                    {
                        auto height = size.height();
                        auto width  = size.width();
                        shape.moveTo(0.0,-height/2.0);
                        shape.lineTo(-width,0.0);
                        shape.lineTo(0.0, height/2.0);
                    }
                    if( dir == PortDir::Output )
                    {
                        auto height = size.height();
                        auto width  = size.width();
                        shape.moveTo(-width,-height/2.0);
                        shape.lineTo(0.0,0.0);
                        shape.lineTo(-width, height/2.0);
                    }
                    break;
                case PortShapeDir::TriConnUp:
                    if( dir == PortDir::Input )
                    {
                        auto height = size.width();
                        auto width  = size.height();
                        shape.moveTo(-width/2,0.0);
                        shape.lineTo(0.0,height);
                        shape.lineTo(width/2.0, 0.0);
                    }
                    if( dir == PortDir::Output )
                    {
                        auto height = size.width();
                        auto width  = size.height();
                        shape.moveTo(-width/2,height);
                        shape.lineTo(0.0,0.0);
                        shape.lineTo(width/2.0, height);
                    }
                    break;
                case PortShapeDir::TriConnDown:
                    if( dir == PortDir::Input )
                    {
                        auto height = size.width();
                        auto width  = size.height();
                        shape.moveTo(-width/2,0.0);
                        shape.lineTo(0.0,-height);
                        shape.lineTo(width/2.0, 0.0);
                    }
                    if( dir == PortDir::Output )
                    {
                        auto height = size.width();
                        auto width  = size.height();
                        shape.moveTo(-width/2,-height);
                        shape.lineTo(0.0,0.0);
                        shape.lineTo(width/2.0,-height);
                    }
                    break;
                }
            };
            shape = QPainterPath();
            switch( portShape )
            {
                case PortShape::Triangular:
                {
                    switch( face )
                    {
                    case Face::Left:
                        makeShape(PortShapeDir::TriConnLeft,dir);
                        break;
                    case Face::Bottom:
                        makeShape(PortShapeDir::TriConnDown,dir);
                        break;
                    case Face::Right:
                        makeShape(PortShapeDir::TriConnRight,dir);
                        break;
                    case Face::Top:
                        makeShape(PortShapeDir::TriConnUp,dir);
                        break;
                    }
                    break;
                }
            }
        }
        static QPainterPath makePortShape(PortShape shape,PortDir dir = PortDir::Input)
        {
            QPainterPath path;
            switch( shape )
            {
                case PortShape::Triangular:
                {
                    if( dir == PortDir::Input )
                    {
                        auto size = StyleBlockShape::connectorSizeGridSizePercent*StyleGrid::gridSize;
                        path.moveTo(0.0,-size.height()/2.0);
                        path.lineTo(size.width(),0.0);
                        path.lineTo(0.0, size.height()/2.0);
                    }
                    if( dir == PortDir::Output )
                    {
                        auto size = StyleBlockShape::connectorSizeGridSizePercent*StyleGrid::gridSize;
                        path.moveTo(size.width(),-size.height()/2.0);
                        path.lineTo(0.0,0.0);
                        path.lineTo(size.width(), size.height()/2.0);
                    }
                    break;
                }
            }
            return path;
        }
        QString toString() const
        {
            return    "Type: "        + port.getType()
                    + " - Instance: " + port.getInstance()
                    + " - idx: "      + QString::number(idx)
                    + " - Face: "     + QString::number(static_cast<uint8_t>(face));
        }
    };
private:    //internal types
    constexpr static uint32_t invalid_index = ~uint32_t(0);
public:
    Block(const QString& typeName = QString(),
          const QString& instanceName = QString(),
          bool selected = false,
          bool sizeable = false);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    QRectF boundingRect() const override { return _boundingRect; }
    void addPort(PortType&& port,
                 Face       face,
                 PortDir    dir   = PortDir::Input,
                 uint32_t   idx   = invalid_index,
                 uint32_t   layer = 0,
                 bool       hide_ = false);
    port_t* portUnderMouse(const QPointF& pos);
    bool mouseOverBlock(const QPointF& pos) const;

    void rotate(bool clockWise);
    void flip(bool vertical);
    void select();
    void deselect();
    void showPorts() const;
    struct
    {

    }_debug;

private:
    //mouse handlers
    void hoverMoveEvent   (QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent  (QGraphicsSceneHoverEvent *event) override;
    void mouseMoveEvent   (QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent  (QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    //painter methods:
    void drawShape       (QPainter* painter) const;
    void drawPorts       (QPainter* painter) const;
    void drawTypeName    (QPainter* painter) const;
    void drawInstanceName(QPainter* painter) const;
    void drawPortHint    (QPainter* painter) const;

    //helpers
    void updateBoundingRect();
    void setBlockEffect(const QColor &color);
    uint32_t nextPortIdx(const Face& face) const;
    void insertPort(PortType&& port,
                    Face       face,
                    PortDir    dir,
                    uint32_t   idx,
                    uint32_t   layer,
                    bool       hide_);
    void updatePorts();

private:
    QString _typeName;
    QString _instanceName;
    QRectF _boundingRect;
    QRectF _draggableRect;
    QPainterPath _blockShape;
    std::vector<port_t> _ports;
    bool _selected;
    bool _sizeable  = false;
    bool _shapeChanged = true;
    bool _hover     = false;

    QPointF _startDrag;
    bool    _dragging = false;
    bool    _hide     = false;
};

//Face helpers:
inline Block::Face& operator++(Block::Face& face)
{
    switch( face )
    {
    case Block::Face::Left:
        face = Block::Face::Bottom;
        break;
    case Block::Face::Bottom:
        face = Block::Face::Right;
        break;
    case Block::Face::Right:
        face = Block::Face::Top;
        break;
    case Block::Face::Top:
        face = Block::Face::Left;
        break;
    }
    return face;
}
inline Block::Face& operator--(Block::Face& face)
{
    switch( face )
    {
    case Block::Face::Left:
        face = Block::Face::Top;
        break;
    case Block::Face::Top:
        face = Block::Face::Right;
        break;
    case Block::Face::Right:
        face = Block::Face::Bottom;
        break;
    case Block::Face::Bottom:
        face = Block::Face::Left;
        break;
    }
    return face;
}
inline Block::Face operator++(Block::Face& face,int)
{
    auto copy = face;
    ++face;
    return copy;
}
inline Block::Face operator--(Block::Face& face,int)
{
    auto copy = face;
    --face;
    return copy;
}
inline std::ostream& operator<<(std::ostream& os,const Block::Face& face)
{
    switch( face )
    {
    case Block::Face::Left:
        return os << "Left";
    case Block::Face::Top:
        return os << "Top";
    case Block::Face::Right:
        return os << "Right";
    case Block::Face::Bottom:
        return os << "Bottom";
    }
}

//Removed because a build-in implementation already exists:
//inline bool operator>(const Block::Face& l,const Block::Face& r)
//{
//    using type = std::underlying_type_t<Block::Face>;
//    return static_cast<type>(l) > static_cast<type>(r);
//}
//
//inline bool operator<(const Block::Face& l,const Block::Face& r)
//{
//    using type = std::underlying_type_t<Block::Face>;
//    return static_cast<type>(l) < static_cast<type>(r);
//}
//
//inline bool operator>=(const Block::Face& l,const Block::Face& r)
//{
//    using type = std::underlying_type_t<Block::Face>;
//    return static_cast<type>(l) >= static_cast<type>(r);
//}
//
//inline bool operator<=(const Block::Face& l,const Block::Face& r)
//{
//    using type = std::underlying_type_t<Block::Face>;
//    return static_cast<type>(l) <= static_cast<type>(r);
//}

} // namespace FlowGUI

#endif // FLOWGUI_BLOCK_H
