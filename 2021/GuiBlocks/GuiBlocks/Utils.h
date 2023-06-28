#ifndef GUIBLOCKS_UTILS_H
#define GUIBLOCKS_UTILS_H

#include <QPointF>
#include "GuiBlocks/Block.h"
#include <cstdint>

namespace GuiBlocks {

//to be used with containers (std::vector, etc)
namespace idx32 { constexpr uint32_t invalid_index = UINT32_MAX; }
double  nextEven(double val);
double  max(double val1,double val2);
QPointF nextGridPosition(QPointF pos,double gridSize);
//QPointF nextGridPositionPort(QPointF pos,Block::PortDir dir,float gridSize);
double  nextGridValue(double val,double gridSize);
QPointF nextEvenGridPosition(QPointF pos,float gridSize);
double  nextEvenGridValue(double val,double gridSize);
double  nextOddGridValue(double val,double gridSize);
bool    isInteger(const double &val);
QPoint  pointConvertion(const QPointF &pointf);
QPointF pointConvertion(const QPoint  &point);
double  squareDistance(const QPointF& p1,const QPointF& p2);
bool    equals(double x1,double x2,double maxDiff=1.0e-6);
QRectF  operator*(const QRectF& rect,double factor);

template<typename T>
std::tuple<T,T> minmax(const std::vector<T>& args)
{
    if( args.size() == 0 )
        throw("minmax empty arg");

    T min = args[0];
    T max = args[0];
    if( args.size() != 1 )
        for( uint32_t idx=1 ; idx<args.size() ; idx++  )
        {
            min = std::min(min,args[idx]);
            max = std::max(max,args[idx]);
        }
    return {min,max};
}
template <typename Head0, typename Head1, typename... Tail>
constexpr auto min(Head0 &&head0, Head1 &&head1, Tail &&... tail)
{
    if constexpr (sizeof...(tail) == 0) {
        return head0 < head1 ? head0 : head1;
    }
    else {
        return min(min(head0, head1), tail...);
    }
}
#define DEBUG_MSG(str) do{static int count=0;qDebug() << str << count++;}while(0)
#define GUI_THROW(str) do{ qDebug() << str << __LINE__ << __FUNCTION__;throw str; }while(0)

} // namespace GuiBlocks

#endif // GUIBLOCKS_UTILS_H
