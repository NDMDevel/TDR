#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "ViewModelBlock.h"
#include "CustomQTableModel.h"

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
//----------------------------------
private:
//    ModelBlock _modelBlock;
    QGraphicsScene _scene;
//    ViewModelBlock _viewModelBlock;
    //CustomQTableModel _tableModel;
};
#endif // MAINWINDOW_H
