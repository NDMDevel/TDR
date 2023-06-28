#ifndef FB_VIEW_H
#define FB_VIEW_H

//FlowGUI
#include "Link.h"
#include "sml_114.hpp"
//#include "StateMachine.h"
#include "Utils.h"
#include "Style.h"
#include "Block.h"

//std
#include <memory>

//Qt
#include <QGraphicsView>
#include <QDebug>
//StateMachine required:
#include <QGuiApplication>
#include <QMouseEvent>
#include <QKeyEvent>

namespace FlowGUI {

using namespace boost::sml;
struct StateMachine;

class View : public QGraphicsView
{
    //friend StateMachine;
public:
    View(QWidget *parent = nullptr);
//    void setStateMachine(sm<StateMachine> *stateM){ _stateMachine = stateM; }

    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    void drawBackground(QPainter* painter, const QRectF &r) override;

    //startLine starts a new line at startPos.
    //updateLine will modify the last line and draw
    //a line that joints startPos and pos with the LinePath
    //
    void startLine(const QPointF& startPos);
    void updateLine(const QPointF& pos);
    void nextLine(const QPointF &pos);
    void cancelLine();
    template<typename T> void setActiveItem(T item)
    {
        _lastItem.item = item;
    }

    //debug
    void drawLink(const QPointF& startPos);
    void updateLink(const QPointF& pos);
    void addBlock(Block* block,const QPointF& centerPos)
    {
        _scene.addItem(block);
        block->setPos(nearestGridPosition(centerPos,StyleGrid::gridSize));
    }
    void moveTo(const QPointF &pos);
    void test();
    void showLastItem();
private:
    QGraphicsScene _scene;
    std::unique_ptr<sm<StateMachine>> _sm;

    struct
    {
        //types:
        using items_t = std::variant<Link*,Block*,Block::port_t*>;
        //members:
        std::optional<items_t> item;
        //methods:
        bool active() const{ return item.has_value(); }
        template<typename T>
        T* get()
        {
            if( !item.has_value() )
                return nullptr;
            if( std::holds_alternative<T*>(item.value()) )
                return std::get<T*>(item.value());
            return nullptr;
        }
    }_lastItem;

    //links related
//    Link::LinePath _linkPath = Link::LinePath::orthogonalThenStraight;

public:
//    struct
//    {
//        Link *link = nullptr;
//        Link::LinePath linePath = Link::LinePath::straight;
//    }_debug;

/*
    //----------------------------------------
    //------ State Machine -------------------
    //----------------------------------------
    //events
    struct pressed     { QMouseEvent *data; };
    struct released    { QMouseEvent *data; };
    struct move        { QMouseEvent *data; const QPoint& prev; };
    struct keyPressed  { QKeyEvent   *data; };
    struct keyreleased { QKeyEvent   *data; };

    auto operator()()
    {
        //guards
        const auto Left   = [](const auto& evt)
        {
            //returns true only if the left mouse btn (with no modifiers)
            //was pressed:

            //button returns all the buttons that triggers the event
            //and will be equals to LeftButton only if the left button
            //was the only button that triggers the event
            const auto& keyboardModifiers = QGuiApplication::queryKeyboardModifiers();
            if(     evt.data->button() == Qt::MouseButton::LeftButton
                &&  keyboardModifiers  == Qt::KeyboardModifier::NoModifier )
                return true;
            return false;
        };
        const auto left   = [](const auto& evt)
        {
            //return true if left mouse btn was pressed regarding modifiers
            //or other btns that may be pressed as well
            return evt.data->buttons() == Qt::MouseButton::LeftButton;
        };
        const auto Right  = [](const auto& evt)
        {
            const auto& keyboardModifiers = QGuiApplication::queryKeyboardModifiers();
            if(     evt.data->button()    == Qt::MouseButton::RightButton
                &&  keyboardModifiers  == Qt::KeyboardModifier::NoModifier )
                return true;
            return false;
        };
        const auto right  = [](const auto& evt)
        {
            return evt.data->buttons() == Qt::MouseButton::RightButton;
        };
        const auto Middle = [](const auto& evt)
        {
            const auto& keyboardModifiers = QGuiApplication::queryKeyboardModifiers();
            if(     evt.data->button()    == Qt::MouseButton::MiddleButton
                &&  keyboardModifiers  == Qt::KeyboardModifier::NoModifier )
                return true;
            return false;
        };
        const auto middle = [](const auto& evt)
        {
            return evt.data->buttons() == Qt::MouseButton::MiddleButton;
        };
        const auto moveIn  = [](const move&   evt)
        {
            return nextGridPosition(evt.data->pos(),StyleGrid::gridSize) == evt.prev;
        };
        const auto moveOut = [](const move&   evt)
        {
            return nextGridPosition(evt.data->pos(),StyleGrid::gridSize) != evt.prev;
        };
        const auto scape   = [](const keyPressed& evt)
        {
            return evt.data->key() == Qt::Key_Escape;
        };

        //actions
        const auto lineTo = [](const auto& evt)
        {
//            auto pos = view->mapFromScene(toQPointF(evt.data->pos()));
//            if( view->_debug.link != nullptr )
//            {
//                view->_debug.link->lineTo(nextGridPosition(pos,StyleGrid::gridSize),
//                                          evt.data->_debug.linePath);
//                view->_debug.link->updateBoundingRect();
//                view->_debug.link->update();
//            }
        };
        const auto show   = [](const auto& evt )
        {
            qDebug() << "show(): " << evt.data->pos();
            qDebug() << QGuiApplication::queryKeyboardModifiers();
        };
        const auto show_1 = [](const auto& evt )
        {
            qDebug() << "moveIn";
        };
        const auto show_2 = [](const auto& evt )
        {
            qDebug() << "moveOut";
        };

        using namespace boost;
        //transitions
        return make_transition_table(
//STATE      + EVENT               [ GUARDS ]     / ACTIONS           = NEXT_STATE
*"init"_s    + sml::event<pressed> [ Left    ]    / show              = "init"_s,
 "init"_s    + sml::event<move>    [ moveIn  ]    / show_1            = "init"_s,
 "init"_s    + sml::event<move>    [ moveOut ]    / lineTo            = "init"_s

        );
    }
*/

};


} // namespace FlowGUI

#endif // FB_VIEW_H
