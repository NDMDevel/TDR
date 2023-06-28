#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "blockviewmodel.h"
#include "blockview.h"
#include "blockmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
//-----------------------------------------------
public Q_SLOTS:
    void blockSizeChanged();
private:
    QGraphicsScene _scene;
    BlockModel _mBlock;
    BlockViewModel _vmBlock;
//    BlockView *block;
};
#endif // MAINWINDOW_H
