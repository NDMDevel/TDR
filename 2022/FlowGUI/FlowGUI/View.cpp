//FlowGUI
#include "View.h"
#include "Style.h"
#include "Utils.h"
#include "Exceptions.h"

//std
#include <cmath>

//Qt
#include <QMouseEvent>

namespace FlowGUI {

using namespace boost::sml;

struct StateMachine
{
public:
    //events
    struct pressed     { View* self; QMouseEvent *data; };
    struct dbleClic    { View* self; QMouseEvent *data; }; //double clic
    struct released    { View* self; QMouseEvent *data; };
    struct move        { View* self; QMouseEvent *data; const QPoint& prevGridPos; };
    struct keyPressed  { View* self; QKeyEvent   *data; };
    struct keyreleased { View* self; QKeyEvent   *data; };
    auto operator()()
    {
        //guards
        const auto Left    = [](const auto& evt)
        {
            //returns true only if the left mouse btn (with no modifiers)
            //was pressed:

            //button returns all the buttons that triggers the event
            //and will be equals to LeftButton only if the left button
            //was the only button that triggers the event
            const auto& keyboardModifiers = QGuiApplication::queryKeyboardModifiers();
            //qDebug() << evt.data->button();
            if(     evt.data->button() == Qt::MouseButton::LeftButton
                &&  keyboardModifiers  == Qt::KeyboardModifier::NoModifier )
                return true;
            return false;
        };
        const auto left    = [](const auto& evt)
        {
            //return true if left mouse btn was pressed regarding modifiers
            //or other btns that may be pressed as well
            bool left1 = evt.data->buttons() == Qt::MouseButton::LeftButton;
            bool left2 = evt.data->button()  == Qt::MouseButton::LeftButton;
            return left1 || left2;
        };
        const auto Right   = [](const auto& evt)
        {
            const auto& keyboardModifiers = QGuiApplication::queryKeyboardModifiers();
            if(     evt.data->button()    == Qt::MouseButton::RightButton
                &&  keyboardModifiers  == Qt::KeyboardModifier::NoModifier )
                return true;
            return false;
        };
        const auto right   = [](const auto& evt)
        {
            bool right1 = evt.data->buttons() == Qt::MouseButton::RightButton;
            bool right2 = evt.data->button()  == Qt::MouseButton::RightButton;
//            return evt.data->buttons() == Qt::MouseButton::RightButton;
            return right1 || right2;
        };
        const auto Middle  = [](const auto& evt)
        {
            const auto& keyboardModifiers = QGuiApplication::queryKeyboardModifiers();
            if(     evt.data->button()    == Qt::MouseButton::MiddleButton
                &&  keyboardModifiers  == Qt::KeyboardModifier::NoModifier )
                return true;
            return false;
        };
        const auto middle  = [](const auto& evt)
        {
            return evt.data->buttons() == Qt::MouseButton::MiddleButton;
        };
        const auto in      = [](const move& evt)
        {
            return nearestGridPosition(evt.data->pos(),StyleGrid::gridSize) == evt.prevGridPos;
        };
        const auto out     = [](const move& evt)
        {
            auto self     = evt.self;
            auto scenePos = self->mapToScene(evt.data->pos());
            auto gridPos  = nearestGridPosition(scenePos,StyleGrid::gridSize);

            return toQPoint(gridPos) != evt.prevGridPos;
        };
        const auto scape   = [](const keyPressed& evt)
        {
            return evt.data->key() == Qt::Key_Escape;
        };
        const auto undo    = [](const keyPressed& evt)
        {
            if(    QGuiApplication::queryKeyboardModifiers() == Qt::ControlModifier
                && (evt.data->key() == 'z' || evt.data->key() == 'Z' ) )
            {
                qDebug() << "CTRL + Z";
                return true;
            }
            return false;
        };
        const auto redo    = [](const keyPressed& evt)
        {
            if(    QGuiApplication::queryKeyboardModifiers() == Qt::ControlModifier
                && (evt.data->key() == 'y' || evt.data->key() == 'Y' ) )
            {
                qDebug() << "CTRL + Y";
                return true;
            }
            return false;
        };
        const auto noItem  = [](const pressed& evt)
        {
            auto self = evt.self;
            auto data = evt.data;
            const auto item = self->itemAt(data->pos());
            return item == nullptr;
        };
        const auto onItem  = [](const pressed& evt)
        {
            auto self = evt.self;
            auto data = evt.data;
            const auto item = self->itemAt(data->pos());
            return item != nullptr;
        };
        const auto onPort  = [](const pressed& evt)
        {
            auto self = evt.self;
            auto data = evt.data;
            const auto itemList = self->items(data->pos());
            for( const auto& item : itemList )
                if( auto block = dynamic_cast<Block*>(item); block != nullptr )
                {
                    auto port = block->portUnderMouse(self->mapToScene(data->pos()));
                    auto ret = port != nullptr;
                    qDebug() << "onPort" << ret;
                    if( ret )
                        self->setActiveItem(port);
                    return port != nullptr;
                }
            qDebug() << "onPort (no item)";
            return false;
        };
        const auto onBlock = [](const pressed& evt)
        {
            auto self = evt.self;
            auto data = evt.data;
            const auto itemList = self->items(data->pos());
            for( const auto& item : itemList )
                if( auto block = dynamic_cast<Block*>(item); block != nullptr )
                {
                    const auto port = block->portUnderMouse(self->mapToScene(data->pos()));
                    if( port == nullptr )
                    {
                        self->setActiveItem(block);
                        return true;
                    }
                    return false;
                }
            return false;
        };
        const auto onLink  = [](const pressed& evt)
        {
            //Iterates over all the items at mouse pos and search for
            //the closest link (if any).
            //As an example, suppose the items found at mouse pos were:
            //link, link, block, link (from top to bottom).
            //The taxi distance to mouse pos will be computed
            //from the two most-top links and the smaller will be recorded.
            //The thierd item (the block) will stop the search for the closest
            //link since it (the block) hides the blocks behind it and so the
            //user cannot selected or interact with them.
            auto self = evt.self;
            auto data = evt.data;
            const auto itemList = self->items(data->pos());
            Link* nearestLink = nullptr;
            double dist = std::numeric_limits<double>::infinity();
            qDebug() << "onLink 0:";
            for( const auto& item : itemList )
            {
                //a block is hidding the links that are behind?
                if( auto block = dynamic_cast<Block*>(item); block != nullptr )
                {
                    if( block->mouseOverBlock(self->mapToScene(data->pos())) )
                        break;
                }
                if( auto link = dynamic_cast<Link*>(item); link != nullptr )
                {
                    auto newDist = link->squareTaxiDistanceTo(self->mapToScene(data->pos()));
                    qDebug() << "onLink 1.1: " << newDist << dist;
                    if( newDist < dist )
                    {
                        dist = newDist;
                        nearestLink = link;
                    }
//                    qDebug() << "onLink 1: " << data->pos() << self->mapToScene(data->pos());
//                    auto newDist = link->squareTaxiDistancePointTo(self->mapToScene(data->pos()));
//                    qDebug() << "onLink 1.1: " << std::get<0>(newDist) << dist << QPointF(std::get<1>(newDist),std::get<2>(newDist));
//                    if( std::get<0>(newDist) < dist )
//                    {
//                        dist = std::get<0>(newDist);
//                        nearestLink = link;
//                    }
                }
                else
                    qDebug() << "no link";
            }
            if( nearestLink == nullptr || dist >= StyleGrid::gridSize )
            {
                qDebug() << "onLink 2: " << nearestLink << dist;
                return false;
            }
            qDebug() << "onLink 3: " << nearestLink << dist;
            return true;
        };
        const auto busyPort= [](const released& evt)
        {
            return false;
        };
        const auto shift   = [](const pressed& evt)
        {
            if( evt.data->modifiers() == Qt::KeyboardModifier::ShiftModifier )
                return true;
            return false;
        };
        const auto ctrl    = [](const pressed& evt)
        {
            if( evt.data->modifiers() == Qt::KeyboardModifier::ControlModifier )
                return true;
            return false;
        };
        const auto is_t    = [](const keyPressed& evt)
        {
            return evt.data->key() == Qt::Key_T;
        };
        const auto is_esc  = [](const keyPressed& evt)
        {
            return evt.data->key() == Qt::Key_Escape;
        };
        const auto moveOut = [](const move& evt)
        {
            auto self = evt.self;
            auto pos = toQPoint(nearestGridPosition(self->mapToScene(evt.data->pos()),StyleGrid::gridSize));
            return evt.prevGridPos != pos;
        };
        //actions
        const auto startLine  = [](const pressed& evt)
        {
            auto self = evt.self;
            self->startLine(nearestGridPosition(self->mapToScene(evt.data->pos()),StyleGrid::gridSize));
        };
        const auto updateLine = [](const auto& evt)
        {
            DEBUG_MSG("updateLine");
            auto self = evt.self;
            self->updateLine(nearestGridPosition(self->mapToScene(evt.data->pos()),StyleGrid::gridSize));
        };
        const auto nextLine   = [](const pressed& evt)
        {
            auto self = evt.self;
            auto pos = nearestGridPosition(self->mapToScene(evt.data->pos()),StyleGrid::gridSize);
            self->nextLine(pos);
        };
        const auto swLine     = [updateLine](const auto& evt)
        {
            Link::switchLinePath();
            //lineTo(evt);
            DEBUG_MSG("swLine()");
            updateLine(evt);
        };
        const auto cancelLine = [](const auto& evt)
        {
            DEBUG_MSG("cancelLine");
            auto self = evt.self;
            self->cancelLine();
        };
        const auto addBlock  = [](const pressed& evt)
        {
            auto self = evt.self;
            static int i = -1;
            static int counter = -1;
            counter++;
            i++;
            switch( counter )
            {
            case 0:
                self->addBlock(new Block("INSTANCE_"+QString::number(i),
                                         "TYPENAME_"+QString::number(i)),
                               self->mapToScene(evt.data->pos()));
                break;
            case 1:
                self->addBlock(new Block("INSTANCE_"+QString::number(i)),
                               self->mapToScene(evt.data->pos()));
                break;
            case 2:
                self->addBlock(new Block(QString(),
                                         "TYPENAME_"+QString::number(i)),
                               self->mapToScene(evt.data->pos()));
                counter = -1;
                break;
            }
        };
        const auto test      = [](const auto& evt)
        {
            evt.self->test();
        };
        const auto selectItem  = [](const pressed& evt)
        {
            auto self = evt.self;
            self->showLastItem();
        };
        const auto deselectAll = [](const pressed& evt)
        {
            auto items = evt.self->items();
            for( auto& item : items  )
            {
                if( auto obj = dynamic_cast<Block*>(item); obj != nullptr )
                    obj->deselect();
            }
        };
        const auto moveItem  = [](const move& evt)
        {
            DEBUG_MSG("moveItem");
        };
        const auto moveTo    = [](const pressed &evt)
        {
            auto &self = evt.self;
            auto &data = evt.data;
            self->moveTo(self->mapToScene(data->pos()));
        };
        //transitions
        using namespace boost;
        return make_transition_table(
//STATE             + EVENT            [ GUARDS            ]   / ACTIONS           = NEXT_STATE
*"waitPress"_s      + event<pressed>   [ left && shift     ]   / addBlock
,"waitPress"_s      + event<pressed>   [ right   &&
                                         onLink  &&
                                         ctrl              ]   / moveTo
,"waitPress"_s      + event<pressed>   [ Left &&
                                         (onBlock||onLink) ]   / selectItem         = "moveItem"_s
,"moveItem"_s       + event<move>      [ moveOut           ]   / moveItem
,"moveItem"_s       + event<released>  [ left              ]                        = "waitPress"_s

,"waitPress"_s      + event<pressed>   [ Left && onPort    ]                        = "validStartLine"_s
,"validStartLine"_s + event<released>  [ left              ]                        = "waitPress"_s
//,"waitPress"_s      + event<pressed>   [ left && !onItem            ]   / deselectAll
,"waitPress"_s      + event<pressed>   [ left && !onItem   ]   / startLine          = "drawLine"_s
,"drawLine"_s       + event<move>      [ moveOut           ]   / updateLine
,"drawLine"_s       + event<pressed>   [ right             ]   / swLine
,"drawLine"_s       + event<pressed>   [ left              ]   / nextLine
,"drawLine"_s       + event<dbleClic>  [ Left              ]                        = "waitPress"_s
,"drawLine"_s       + event<keyPressed>[ is_esc            ]   / cancelLine         = "waitPress"_s
,"waitPress"_s      + event<keyPressed>[ is_t              ]   / test
//,"waitPress"_s      + event<move>                                 / showPos
//*"waitPress"_s      + event<pressed>   [ Left                 ]   / setTargetItems    = "trgAction"_s
//,"triggerAction"_s  + event<released>  [ Left && canStartLine ]   / startLine         = "waitPress"_s
//,"triggerAction"_s  + event<move>      [ moveOut &&  noItem ]   / showActiveItem    = "waitRelease"_s
//,"triggerAction"_s  + event<move>      [ moveOut && !noItem ]   / moveActiveItem    = "moveItem"_s
//,"moveItem"_s       + event<released>  [ Left               ]                       = "waitPress"_s
//,"triggerAction"_s  + event<released>  [ Left               ]   / startNewLine      = "updateLine"_s
//,"updateLine"_s     + event<move>      [ moveOut            ]   / updateLine
//,"updateLine"_s     + event<released>  [ Left               ]                       = "waitPress"_s

,"waitPress"_s      + sml::on_entry<_>                          / [](){qDebug() << " -> waitPress";}
//,"waitPress"_s      + sml::on_exit<_>                           / [](){qDebug() << "waitPress";}
,"waitRelease"_s    + sml::on_entry<_>                          / [](){qDebug() << " -> waitRelease";}
//,"waitRelease"_s    + sml::on_exit<_>                           / [](){qDebug() << "waitRelease";}
,"triggerAction"_s  + sml::on_entry<_>                          / [](){qDebug() << " -> triggerAction";}
//,"triggerAction"_s  + sml::on_exit<_>                           / [](){qDebug() << "triggerAction";}
,"moveItem"_s       + sml::on_entry<_>                          / [](){qDebug() << " -> moveItem";}
//,"moveItem"_s       + sml::on_exit<_>                           / [](){qDebug() << "moveItem";}
,"updateLine"_s     + sml::on_entry<_>                          / [](){qDebug() << " -> updateLine";}
//,"updateLine"_s     + sml::on_exit<_>                           / [](){qDebug() << "updateLine";}
,"validStartLine"_s + sml::on_entry<_>                          / [](){qDebug() << " -> validStartLine";}

        );
    }
};

//this macro was created just to avoid missindentation of QtCreator
#define SM_CTOR std::make_unique<sm<StateMachine>>()
View::View(QWidget *parent) : QGraphicsView(parent),_scene(parent),_sm{SM_CTOR}
{
    setOptimizationFlag(QGraphicsView::DontSavePainterState);
    setRenderHints(QPainter::Antialiasing |
                   QPainter::SmoothPixmapTransform);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setCacheMode(QGraphicsView::CacheBackground);

//    setResizeAnchor(ViewportAnchor::NoAnchor);
//    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
//    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    //setSceneRect();

    QBrush brush;
    brush.setColor(StyleGrid::backgroundGridColor);
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    setBackgroundBrush(brush);

    //call setItemIndexMethod with NoIndex is necessary to void crashes
    //when deleting (manually) the links removed from the scene.
    //It seems that the scene keeps reference to the links
    //after removing it in the BSPItemIndexMethod
    _scene.setItemIndexMethod(QGraphicsScene::ItemIndexMethod::NoIndex);
    QGraphicsView::setScene(&_scene);

}
void View::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    auto pos = mapToScene(event->pos());
    static QPoint prev = toQPoint(nearestGridPosition(pos,StyleGrid::gridSize));
    _sm->process_event(StateMachine::move{.self=this,.data=event,.prevGridPos=prev});
    prev = toQPoint(nearestGridPosition(pos,StyleGrid::gridSize));
//    emit mousePos(event->pos(),prev);
}
void View::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    _sm->process_event(StateMachine::pressed{.self=this,.data=event});
}
void View::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    //event->button()
    _sm->process_event(StateMachine::released{.self=this,.data=event});
}
void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
    _sm->process_event(StateMachine::dbleClic{.self=this,.data=event});
}
void View::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    _sm->process_event(StateMachine::keyPressed{.self=this,.data=event});
}
void View::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyReleaseEvent(event);
    _sm->process_event(StateMachine::keyreleased{.self=this,.data=event});
}
void View::drawBackground(QPainter* painter, const QRectF &r)
{
    QGraphicsView::drawBackground( painter , r );

    auto drawGrid = [&](double gridStep)
    {
        QRect   windowRect = rect();
        QPointF tl = mapToScene(windowRect.topLeft());
        QPointF br = mapToScene(windowRect.bottomRight());

        double left   = std::floor(tl.x() / gridStep - 0.5);
        double right  = std::floor(br.x() / gridStep + 1.0);
        double bottom = std::floor(tl.y() / gridStep - 0.5);
        double top    = std::floor (br.y() / gridStep + 1.0);

        // vertical lines
        for (int xi = int(left); xi <= int(right); ++xi)
        {
            QLineF line(xi * gridStep, bottom * gridStep,
                        xi * gridStep, top * gridStep );

            painter->drawLine(line);
        }

        // horizontal lines
        for (int yi = int(bottom); yi <= int(top); ++yi)
        {
            QLineF line(left * gridStep, yi * gridStep,
                        right * gridStep, yi * gridStep );
            painter->drawLine(line);
        }
    };

    //  auto const &flowViewStyle = StyleCollection::flowViewStyle();

    QPen pfine(StyleGrid::fineGridColor,
               qreal(StyleGrid::fineGridWidth));

    painter->setPen(pfine);
    drawGrid(qreal(StyleGrid::gridSize));

    QPen p(StyleGrid::coarseGridColor,
           qreal(StyleGrid::coarseGridWidth));

    painter->setPen(p);
    drawGrid(10.0*qreal(StyleGrid::gridSize));
}
void View::startLine(const QPointF &startPos)
{
    auto link = new Link(startPos);
    _lastItem.item = link;
    _scene.addItem(link);
    link->update();
}
void View::updateLine(const QPointF &pos)
{
    if( !_lastItem.active() )
        return;
    if( auto link = _lastItem.get<Link>(); link != nullptr )
    {
        link->lineTo(pos);
        link->updateBoundingRect();
        link->update();
    }
}
void View::nextLine(const QPointF &pos)
{
    if( !_lastItem.active() )
        return;
    if( auto link = _lastItem.get<Link>(); link != nullptr )
    {
        link->nextLine(pos);
//        link->lineTo(pos);
//        link->updateBoundingRect();
//        link->update();
    }
}
void View::cancelLine()
{
    if( !_lastItem.active() )
        return;
    if( auto link = _lastItem.get<Link>(); link != nullptr )
    {
        link->removeLastLine();
        if( link->size() == 0 )
            _scene.removeItem(link);
        else
        {
            link->updateBoundingRect();
            link->update();
        }
    }
}
void View::moveTo(const QPointF &pos)
{
    if( !_lastItem.active() )
        return;
    if( auto link = _lastItem.get<Link>(); link != nullptr )
    {
        auto moveData = link->prepareMoveTo(pos);
        link->moveTo(moveData);
    }
}
void View::test()
{
    static int count = 0;
    auto block = new Block("Test","Test"+QString::number(count));
    _scene.addItem(block);
    block->setPos(nearestGridPosition({0.0,0.0},StyleGrid::gridSize));

    auto port = PortType("port","port"+QString::number(count));
    block->addPort(std::move(port),Block::Face::Left,Block::PortDir::Input,3);
    count++;
    block->addPort(std::move(port),Block::Face::Left,Block::PortDir::Output);
    count++;
    block->addPort(std::move(port),Block::Face::Right,Block::PortDir::Input);
    count++;
    block->addPort(std::move(port),Block::Face::Right,Block::PortDir::Output);
    count++;
    block->addPort(std::move(port),Block::Face::Bottom,Block::PortDir::Input);
    count++;
    block->addPort(std::move(port),Block::Face::Bottom,Block::PortDir::Output);
    count++;
    block->addPort(std::move(port),Block::Face::Top,Block::PortDir::Output);
    count++;
    block->addPort(std::move(port),Block::Face::Top,Block::PortDir::Input);
    count++;
}
void View::showLastItem()
{
    if( !_lastItem.active() )
        qDebug() << "_lastItem: empty";
    qDebug() << "_lastItem: " << _lastItem.item.value().index();
}

} // namespace FlowGUI
