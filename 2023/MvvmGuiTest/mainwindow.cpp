#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ViewBlock.h"
#include "ModelBlock.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
//    , _viewModelBlock(_modelBlock)
{
    ui->setupUi(this);
    ui->mainView->setScene(&_scene);

//    ViewBlock* block = new ViewBlock(_viewModelBlock);
//    _scene.addItem(block);

/*    _tableModel.append({"Ford","2021","12565487"});
    _tableModel.append({"Renault","2022","df32h9ig"});
    _tableModel.append({"Chevrolet","2019","fi39osoie23"});
    ui->tableView->setModel(&_tableModel);
//    ui->tableView->resizeColumnsToContents();
*/
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

MainWindow::~MainWindow()
{
    delete ui;
}

