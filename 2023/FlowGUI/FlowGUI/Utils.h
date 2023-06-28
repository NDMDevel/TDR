#ifndef FLOWGUI_UTILS_H
#define FLOWGUI_UTILS_H

#include <QPointF>
#include <QString>
#include <type_traits>>

namespace FlowGUI {

QPoint  toQPoint (const QPointF& pos);
QPointF toQPointF(const QPoint&  pos);
QPointF nearestGridPosition(QPointF pos, double gridSize);
double  nextGridValue(double val, double gridSize,bool positive=true);
double  nextOddGridValue(double val, double gridSize);
double  nearestGridValue(double val,double gridSize);
double  squareDistance(const QPointF &p1, const QPointF &p2);
bool    equals(double x1, double x2, double maxDiff=1.0e-6);
bool    equals(const QPointF &p1, const QPointF &p2, const QPointF maxDiff=QPointF(1.0e-6,1.0e-6));
template <typename T> double signum(T val){return (T(0) < val) - (val < T(0));}
//QPointF nearestGridPos(const QPointF& pos,double gridSize);
#define DEBUG_MSG(str) do{static int count=0;qDebug() << str << count++;}while(0)

template<typename P>
QString pointToQString(const P& p)
{
    static_assert(std::is_same_v<P,QPoint> ||
                  std::is_same_v<P,QPointF>,"template arg must be QPoint or QPointF");
    return QString("(%1 %2)").arg(QString::number(p.x()),QString::number(p.y()));
}

} // namespace FlowGUI

#endif // FLOWGUI_UTILS_H
