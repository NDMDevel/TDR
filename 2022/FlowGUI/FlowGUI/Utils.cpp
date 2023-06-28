#include "Utils.h"
#include <cmath>

namespace FlowGUI {

QPoint  toQPoint(const QPointF &pos)
{
    QPoint p;
    p.setX(std::lround(pos.x()));
    p.setY(std::lround(pos.y()));
    return p;
}
QPointF toQPointF(const QPoint &pos)
{
    QPointF p;
    p.setX(pos.x());
    p.setY(pos.y());
    return p;
}
QPointF nearestGridPosition(QPointF pos, double gridSize)
{
    pos /= gridSize;
//    pos.setX(pos.x()/gridSize);
//    pos.setY(pos.y()/gridSize);
    return pos.toPoint()*gridSize;
}
double nextGridValue(double val, double gridSize,bool positive)
{
    int count = int(val/gridSize);
    double res;
    if( positive )
        do
            res = double(count++)*gridSize;
        while( res-val<=0.0 );
    else
        do
            res = double(count--)*gridSize;
        while( val-res<=0.0 );
    return res;
}
double nextOddGridValue(double val, double gridSize)
{
    int count = int(val/gridSize);
    double res;
    do
        res = double(count++)*gridSize;
    while( res-val<=0.0 || (count-1)%2==0 );
    return res;
}
double nearestGridValue(double val,double gridSize)
{
    int count = int(val/gridSize);
    int nextCount = count + 1;
    if( val < 0.0 )
        nextCount = count - 1;
    if( std::abs(count*gridSize-val) < std::abs(nextCount*gridSize-val) )
        return count*gridSize;
    return nextCount*gridSize;
}
double squareDistance(const QPointF &p1, const QPointF &p2)
{
    return std::pow(p1.x()-p2.x(),2.0) + std::pow(p1.y()-p2.y(),2.0);
}
bool equals(double x1, double x2, double maxDiff)
{
    return std::abs(x1-x2) < maxDiff;
}
bool equals(const QPointF &p1, const QPointF &p2, const QPointF maxDiff)
{
    return equals(p1.x(),p2.x(),maxDiff.x()) && equals(p1.y(),p2.y(),maxDiff.y());
}

} // namespace FlowGUI
