#ifndef FLOWGUI_STATEMACHINE_H
#define FLOWGUI_STATEMACHINE_H

//#include "View.h"

#include "sml_114.hpp"
#include "Utils.h"
#include "Style.h"

#include <QGuiApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

namespace FlowGUI {

#define STATE_MACHINE
#ifdef STATE_MACHINE

using namespace boost::sml;
class View;

struct StateMachine
{
public:
    //events
    struct pressed     { QMouseEvent *data; };
    struct released    { QMouseEvent *data; };
    struct move        { View* self; QMouseEvent *data; const QPoint& prev; };
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
            return nearestGridPosition(evt.data->pos(),StyleGrid::gridSize) == evt.prev;
        };
        const auto moveOut = [](const move&   evt)
        {
            return nearestGridPosition(evt.data->pos(),StyleGrid::gridSize) != evt.prev;
        };
        const auto scape   = [](const keyPressed& evt)
        {
            return evt.data->key() == Qt::Key_Escape;
        };

        //actions
        const auto lineTo = [&](const auto& evt)
        {
            auto self = const_cast<FlowGUI::View*>(evt.self);
            qDebug() << "no const: " << self;
            qDebug() << evt.self;
//            auto pos = self->mapFromScene(toQPointF(evt.data->pos()));
//            if( evt.self->_debug.link != nullptr )
//            {
//                evt.self->_debug.link->lineTo(nextGridPosition(pos,StyleGrid::gridSize),
//                                          evt.data->_debug.linePath);
//                evt.self->_debug.link->updateBoundingRect();
//                evt.self->_debug.link->update();
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

        //transitions
        return make_transition_table(

//STATE      + EVENT          [ GUARDS ]          / ACTIONS           = NEXT_STATE
*"init"_s    + event<pressed> [ Left    ]         / show              = "init"_s,
 "init"_s    + event<move>    [ moveIn  ]         / show_1            = "init"_s,
 "init"_s    + event<move>    [ moveOut ]         / lineTo            = "init"_s

        );
    }
};
#endif  //STATE_MACHINE
} // namespace FlowGUI

#endif // FLOWGUI_STATEMACHINE_H
