#include "mainwindow.h"

#include <QApplication>
//#include "Utils.h"
//#include <tuple>

int main(int argc, char *argv[])
{
    /*
    const auto &perp = [](const QPointF &p1,const QPointF &p2)
    {
        auto x1 = p1.x();
        auto x2 = p2.x();
        auto y1 = p1.y();
        auto y2 = p2.y();
        auto pp1 = QPointF(y2,x1);
        auto pp2 = QPointF(y1,x2);
        return std::make_tuple<const QPointF&,const QPointF&>(pp1,pp2);
    };
    const auto &show = [](const QPointF &p1,const QPointF &p2,
                          const QPointF &pp1,const QPointF &pp2)
    {
        QLineF line(p1,p2);
        QLineF pline(pp1,pp2);
//        line.intersect(pline,nullptr);
        qDebug() << line.intersect(pline,nullptr) << p1 << p2 << pp1 << pp2
                 << FlowGUI::squareDistance(p1,pp1)
                 << FlowGUI::squareDistance(p1,pp2)
                 << FlowGUI::squareDistance(p2,pp1)
                 << FlowGUI::squareDistance(p2,pp2)
                 << FlowGUI::squareDistance(p1,p2)
                 << FlowGUI::squareDistance(pp1,pp2);
    };
    std::srand(std::time(nullptr));
    for( size_t idx=0 ; idx<1000 ; idx++ )
    {
        QPointF p1(std::rand()%1000,std::rand()%1000);
        QPointF p2(std::rand()%1000,std::rand()%1000);
        auto[pp1,pp2] = perp(p1,p2);
        show(p1,p2,pp1,pp2);
    }
    QPointF p1(-5,-4);
    QPointF p2(-2,-3);
    QPointF pp1(-3,-5);
    QPointF pp2(-4,-2);
    qDebug() << FlowGUI::squareDistance(p1,pp1);
    qDebug() << FlowGUI::squareDistance(p1,pp2);
    qDebug() << FlowGUI::squareDistance(p2,pp1);
    qDebug() << FlowGUI::squareDistance(p2,pp2);
    qDebug() << FlowGUI::squareDistance(p1,p2);
    qDebug() << FlowGUI::squareDistance(pp1,pp2);
    return 0;*/
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
