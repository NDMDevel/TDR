#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//USER Includes BEGINS
//#include "sml_114.hpp"
#include "FlowGUI/View.h"
//#include "FlowGUI/StateMachine.h"
#include <QTreeWidgetItem>
//USER Includes ENDS

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

//USER Begins
public slots:
    //main menu
    void exitApp();
    //debug
    void drawTestLine();

private:

private:
//USER Ends
};
#endif // MAINWINDOW_H
