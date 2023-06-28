#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "customgraphicsview.h"
#include "baseblock.h"
#include "gridgraphicsitem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void paintEvent(QPaintEvent *e) override;
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    BaseBlock *block;
    GridGraphicsItem *grid;

    QRectF mainWindowRect;

protected slots:
    void testAddBlock();
    void action(){ qDebug() << "action triggered!!";}
    void menuClicked(QAction *action);

};
#endif // MAINWINDOW_H
