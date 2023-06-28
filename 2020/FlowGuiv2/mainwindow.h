#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>

#include "customrect.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void StartLine(const QPoint& point);
    void UpdateEndLine(const QPoint& point);
    void MouseRelease(const QPoint&);

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QGraphicsRectItem *rectangle;
    QGraphicsEllipseItem *ellipse;
    CustomRect *customRect;

    QPointF startLinePos;
    QGraphicsLineItem *line;
    bool lineStart = false;
};

#endif // MAINWINDOW_H
