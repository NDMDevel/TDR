#ifndef FB_VIEW_H
#define FB_VIEW_H

//FlowGUI
#include "Utils.h"

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

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View(QWidget *parent = nullptr);

    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    void drawBackground(QPainter* painter, const QRectF &r) override;

    //debug
signals:
    void mouseMoved(QPoint pos,QPointF scenePos);
private:
    QGraphicsScene _scene;
};


} // namespace FlowGUI

#endif // FB_VIEW_H
