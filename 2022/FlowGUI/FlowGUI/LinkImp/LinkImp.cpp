//FlowGUI
#include "LinkImp/LinkImp.h"
#include "Utils.h"
#include "Style.h"

//std
#include <utility>

//Qt
#include <QDebug>

namespace FlowGUI {

LinkImp::LinePath LinkImp::_path = LinkImp::LinePath::orthogonalThenStraight;

LinkImp::LinkImp(const QPointF& pos) noexcept
{
    _nodes.reserve(3);
    _nodes.emplace_back(Node{pos});
    _size    = 1;
    _rootIdx = 0;
    _srcIdx  = 0;
}
LinkImp::MoveType LinkImp::prepareMoveTo(const QPointF &pos)
{
    (void)pos;
    return {};
}
void LinkImp::moveTo(const MoveType &movePos)
{
    auto &pos = movePos._movePos;
    if( pos.has_value() )
        qDebug() << "has_value()";
    else
        qDebug() << "!has_value()";
}
void LinkImp::lineTo(const QPointF& pos,Port *port)
{
//    if( _endIdx != invalid_index )
//    {
//        _srcIdx = _endIdx;
//        _endIdx = invalid_index;
//        _midIdx = invalid_index;
//    }
    if( auto midPos = computeMidPoint(_nodes[_srcIdx].coord,pos,_path) )
    {
        if( _midIdx == invalid_index )
        {
            if( _endIdx == invalid_index )
                _midIdx = createNode(midPos.value(),_srcIdx,port);
            else
            {
                _midIdx = _endIdx;
                _endIdx = invalid_index;
                _nodes[_midIdx].coord = midPos.value();
            }
        }
        else
            _nodes[_midIdx].coord = midPos.value();
    }
    else
        if( _midIdx != invalid_index && _endIdx != invalid_index )
        {
            //clear endIdx and use _midIdx as endIdx
            removeNode(_endIdx);
            _endIdx = _midIdx;
            _midIdx = invalid_index;
        }
    if( _endIdx == invalid_index )
    {
        if( _midIdx == invalid_index )
            _endIdx = createNode(pos,_srcIdx,port);
        else
            _endIdx = createNode(pos,_midIdx,port);
    }
    else
        _nodes[_endIdx].coord = pos;
}
void LinkImp::nextLine(const QPointF &pos, LinkImp::Port *port)
{
    if( _endIdx != invalid_index )
        if( _nodes[_endIdx].coord == pos )
        {
            _srcIdx = _endIdx;
            _midIdx = invalid_index;
            _endIdx = invalid_index;
            lineTo(pos,port);
        }
}
void LinkImp::removeLastLine()
{
    if( _endIdx != invalid_index )
    {
        removeNode(_endIdx);
        _endIdx = invalid_index;
    }
    if( _midIdx != invalid_index )
    {
        removeNode(_midIdx);
        _midIdx = invalid_index;
    }
}
void LinkImp::switchLinePath()
{
    switchLinePath(_path);
}
LinkImp::Idx LinkImp::createNode(const QPointF& coord,const Idx& linkTo,Port* port) noexcept
{
    for( Idx i=0 ; i<size() ; i++ )
        if( _nodes[i].isEmpty() )
        {
            _nodes[i] = Node{coord,linkTo,port};
            if( linkTo != invalid_index )
                _nodes[linkTo].status.childs++;
            _size++;
            return i;
        }
    //control arrives here if there is no space in _nodes to store
    //another node, so a resize is needed
    _nodes.emplace_back(Node{coord,linkTo,port});
    if( linkTo != invalid_index )
        _nodes[linkTo].status.childs++;
    _size++;
    return _nodes.size()-1; //index of the last emplaced node
}
void LinkImp::removeNode(Idx idx) noexcept
{
    if( idx >= _nodes.size() )
    {
        FLOWGUI_REPORT(IndexOutOfRange());
        return;
    }
    if( _nodes[idx].isEmpty() == false )
    {
        auto linkTo = _nodes[idx].linkTo;
        _nodes[idx].makeEmpty();
        _size--;
        if( linkTo != invalid_index )
        {
            if( _nodes[linkTo].status.childs > 0 )
                _nodes[linkTo].status.childs--;
            else
            {
                //the node linkTo was already disconnected
                FLOWGUI_REPORT(UnconnectedNode());
            }
        }
    }
    else
    {
        FLOWGUI_REPORT(AlreadyRemoved());
        return;
    }
}
//select node at pos, or nodes under selArea, the selected
//flag node will be set
bool LinkImp::select(const QPointF& pos)
{
    (void)pos;
    return false;
}
bool LinkImp::select(const std::vector<QPointF>& selArea)
{
    (void)selArea;
    return false;
}
void LinkImp::clearSelection()
{
    for( auto& node : _nodes )
        if( node.isSelected() )
            node.deselect();
}
void LinkImp::displaceSelection(const QPointF& delta)
{
    (void)delta;
}
std::tuple<double,double,double> LinkImp::squareTaxiDistancePointTo(const QPointF& pos) const
{
    const auto& idx = getNearestPoint(pos);
    if( auto index = std::get_if<Idx>(&idx); index != nullptr )
        return {squareDistance(_nodes[*index].coord,pos),_nodes[*index].coord.x(),_nodes[*index].coord.y()};
    else
    {
        auto x = std::get<2>(std::get<1>(idx));
        auto y = std::get<3>(std::get<1>(idx));
        return {squareDistance(QPointF(x,y),pos),x,y};
    }
}
double LinkImp::squareTaxiDistanceTo(const QPointF& pos) const
{
    const auto& idx = getNearestPoint(pos);
    if( auto index = std::get_if<Idx>(&idx); index != nullptr )
        return squareDistance(_nodes[*index].coord,pos);
    else
    {
        auto x = std::get<2>(std::get<1>(idx));
        auto y = std::get<3>(std::get<1>(idx));
        return squareDistance(QPointF(x,y),pos);
    }
}
std::variant<LinkImp::Idx,std::tuple<LinkImp::Idx,LinkImp::Idx,double,double>>
    LinkImp::getNearestPoint(const QPointF &pos) const
{
    //first (to avoid heavy computations) verify if pos is one of the link nodes,
    //if so, returns the index of the node
    auto nearestIdx = [&]
    {
        NodeIter niter(*this);
        const auto idx = niter.idx;
        while( true )
        {
            if( const auto& node=iterateCoords(niter); node.has_value() )
            {
                const auto& coord = node.value();
                if( equals(pos,coord) )
                    return idx;
            }
            else
                return invalid_index;
        }
    }();
    if( nearestIdx != invalid_index )
        return nearestIdx;

    //conpute the distance from pos to each node of the link and
    //records the min distance and index of the node
    auto squareDist = std::numeric_limits<double>::infinity();
    {
        NodeIter iter(*this);
        while( true )
        {
            auto iterIdx = iter.idx;
            if( const auto &idx=iterateCoords(iter); idx.has_value() )
            {
                auto coord = idx.value();
                if( auto dist = squareDistance(coord,pos); dist < squareDist )
                {
                    squareDist = dist;
                    nearestIdx = iterIdx;
                }
            }
            else
                break;
        }
    }

    //now iterates over the lines of the link, recording the
    //(square) distance between pos and the intersection of
    //the current line of the link with its perpendicular that
    //contanins pos. If there is no intersection, then nothing
    //will be recorded (and an empty std::optional will be returned)
    PairIter iter(*this);
    Idx nearestIdx1 = invalid_index;
    Idx nearestIdx2 = invalid_index;
    QPointF nearestInterPoint;
    while( true )
    {
        if( const auto &idx=iterateIndex(iter); idx.has_value() )
        {
            const auto& getPerp = [](const QLineF& line,const QPointF& pos)
            {
                const auto& perp = [](const QPointF &p1,const QPointF &p2)
                {
                    auto x1 = p1.x();
                    auto x2 = p2.x();
                    auto y1 = p1.y();
                    auto y2 = p2.y();
                    auto pp1 = QPointF(y2,x1);
                    auto pp2 = QPointF(y1,x2);
                    return std::make_tuple<const QPointF&,const QPointF&>(pp1,pp2);
                };
                if( line.dx() == 0 )//vertical line?
                {
                    auto xmin = std::min(line.p1().x(),pos.x())-StyleGrid::gridSize;
                    auto xmax = std::max(line.p1().x(),pos.x())+StyleGrid::gridSize;
                    auto p1 = QPointF(xmin,pos.y());
                    auto p2 = QPointF(xmax,pos.y());
                    return QLineF(p1,p2);
                }
                auto[p1,p2] = perp(line.p1(),line.p2());
                QLineF pline = QLineF(p1,p2);
                pline.translate(pos-p1);
                return pline;
            };
            auto[idx1,idx2] = idx.value();
            const auto& p1 = _nodes[idx1].coord;
            const auto& p2 = _nodes[idx2].coord;
            QLineF line(p1,p2);
            auto pLine = getPerp(line,pos);
            QPointF pInter;
            switch( line.intersect(pLine,&pInter) )
            {
                case QLineF::IntersectType::NoIntersection:
                    break;
                case QLineF::IntersectType::UnboundedIntersection:
                    {
                        auto[xmin,xmax] = std::minmax(line.p1().x(),line.p2().x());
                        auto[ymin,ymax] = std::minmax(line.p1().y(),line.p2().y());
                        QRectF rect(QPointF(xmin,ymax),QPointF(xmax,ymin));
                        if( rect.contains(pInter) )
                        {
                            //the intersection is Bounded
                            if( auto dist = squareDistance(pos,pInter); dist < squareDist )
                            {
                                nearestIdx1  = idx1;
                                nearestIdx2  = idx2;
                                nearestInterPoint = pInter;
                                squareDist = dist;
                            }
                        }
                    }
                    break;
                case QLineF::IntersectType::BoundedIntersection:
                    if( auto dist = squareDistance(pos,pInter); dist < squareDist )
                    {
                        nearestIdx1  = idx1;
                        nearestIdx2  = idx2;
                        nearestInterPoint = pInter;
                        squareDist = dist;
                    }
                    break;
            }
        }
        else
        {
            if( nearestIdx1 != invalid_index )
                return std::tuple<Idx,Idx,double,double>(nearestIdx1,nearestIdx2,nearestInterPoint.x(),nearestInterPoint.y());
            break;
        }
    }
    return nearestIdx;
}
std::tuple<LinkImp::Idx,LinkImp::Idx> LinkImp::getGrabbedIndexs(const QPointF &pos) const
{
    const auto belongsToLine = [&](Idx idxP1,Idx idxP2,const QPointF &point) -> std::optional<std::tuple<LinkImp::Idx,LinkImp::Idx>>
    {
        auto p1 = _nodes[idxP1].coord;
        auto p2 = _nodes[idxP2].coord;
        auto[minx,maxx] = std::minmax(p1.x(),p2.x());
        auto[miny,maxy] = std::minmax(p1.y(),p2.y());

        //the rectangle with top-left = p1, and botton-right = p2 will be the working area
        //and the point should be inside this rectangle, if not, then point is not on the
        //trajectory between p1 and p2
        if( !(point.x() < minx || point.x() > maxx || point.y() < miny || point.y() > maxy) )
        {
            //if point is exactly a node, return the index of that node and invalid_index
            if( p1 == point )
                return {{idxP1,invalid_index}};
            if( p2 == point )
                return {{invalid_index,idxP2}};

            //now checks if point is on the trajectory joining p1 and p2
            auto dx = p2.x()-p1.x();
            //corner case: the line is vertical (ie, dx = 0)
            if( dx == 0.0 && equals(point.x(),p1.x()) )
                return {{idxP1,idxP2}};
            else
            {
                //the line is not vertical.
                //now we compute the coefficients of the line joining p1 and p2:
                // y = a*x + b
                auto a = (p2.y()-p1.y())/dx;
                auto b = -a*p1.x()+p1.y();
                //point is on the line if: point.y == a*point.x + b
                if( equals( point.y() , a*point.x()+b ) )
                    return {{idxP1,idxP2}};
            }
        }
        return {};
    };
    if( _nodes.size() < 2 )
        return {invalid_index,invalid_index};
    //Computes the distances from pos to the nearest point and line,
    //if the line is closer to pos, its indexes will be return, otherwise
    //the pos's indexes will be returned.

    //distances from pos to a node (pdist) and to a line (ldist)
    auto pdist = qInf();
    auto ldist = qInf();
    //index of the node that is closest to pos
    auto pIdx  = invalid_index;
    //indexes of the line that is closest to pos
    auto lIdx1 = invalid_index;
    auto lIdx2 = invalid_index;
    QPointF inter;

    PairIter iter(*this);
    while( auto idx = iterateIndex(iter) )
    {
        const auto[from,to] = idx.value();
        //compute the distances from pos to p1 and p2
        const auto& p1 = _nodes[from].coord;
        const auto& p2 = _nodes[to].coord;
        if( pdist > squareDistance(pos,p1) )
        {
            //records the min distance and the index
            pdist = squareDistance(pos,p1);
            pIdx  = from;
        }
        if( pdist > squareDistance(pos,p2) )
        {
            //records the min distance and the index
            pdist = squareDistance(pos,p2);
            pIdx  = to;
        }
        //now compute the distance from pos to the line
        //that is formed by p1 and p2.
        //This distance is taken from the perpendicular line that
        //connects pos with the target line (formed by p1 and p2)
        QLineF line(p1,p2);
        QPointF pp1( pos.x()+line.dy() , pos.y()-line.dx() );
        QPointF pp2( pos.x()-line.dy() , pos.y()+line.dx() );
        QLineF pline(pp1,pp2);

        switch( line.intersect(pline,&pp1) )
        {
            case QLineF::UnboundedIntersection:
                //if the unbounded intersection point is part of line then
                //compute the distance between pos a pp1
                if( belongsToLine(from,to,pp1) )
                    if( ldist > squareDistance(pos,pp1) )
                    {
                        ldist = squareDistance(pos,pp1);
                        lIdx1 = from;
                        lIdx2 = to;
                        inter = pp1;
                    }
                break;
            case QLineF::BoundedIntersection:
                if( ldist > squareDistance(pos,pp1) )
                {
                    ldist = squareDistance(pos,pp1);
                    lIdx1 = from;
                    lIdx2 = to;
                    inter = pp1;
                }
                break;
            case QLineF::NoIntersection:
                qDebug() << "********************** this represents a throw (since we are using perpendicular lines, should allways be an interseccion)" << __LINE__;
                break;
        }
    }
    //if no line was selected, then returns the pIdx
    if( lIdx1 == invalid_index )
        return {pIdx,invalid_index};

    //only select the closest line if pos is near the center of the line:
    //Lenght of the line
    const auto len = squareDistance(_nodes[lIdx1].coord,_nodes[lIdx2].coord);
    //distance from intersecction to nearest extreme
    auto lIdx  = lIdx1;
    auto edist = squareDistance(inter,_nodes[lIdx1].coord);
    if( edist > squareDistance(inter,_nodes[lIdx2].coord) )
    {
        edist = squareDistance(inter,_nodes[lIdx2].coord);
        lIdx = lIdx2;
    }
    if( edist < len*0.04 )  //0.2^2 = 0.04 -> this represents de 20% of the lenght
    {
        //this means that pos is closer to the extreme (lIdx) than is
        //to the center of the line.
        //Now the closest node (pIdx or lIdx) will be selected
        if( squareDistance(pos,_nodes[lIdx].coord) < pdist )
            return {lIdx,invalid_index};
        return {pIdx,invalid_index};
    }
    if( pdist < ldist )
        return {pIdx,invalid_index};
    return {lIdx1,lIdx2};

}
LinkImp::Idx  LinkImp::size() const { return _size; }

//Node& operator[](Idx pos);
//iterator:
void LinkImp::resetIter(PairIter &iter) const
{
    if( _size < 2 )
    {
        iter.reset();
        return;
    }
    iter.to = _rootIdx;
    for( Idx i=0 ; i<_nodes.size() ; i++ )
        if( _nodes[i].isEmpty() == false && _nodes[i].linkTo == _rootIdx )
        {
            iter.from  = i;
            iter.count = _nodes[_rootIdx].status.childs;
            return;
        }
    //this code should not be reached
    FLOWGUI_REPORT(IllFormedLink());
}
void LinkImp::iterate(PairIter &iter) const
{
    //need to update 'to'?
    if( iter.count >= 1 )
    {
        for( Idx i=iter.to+1 ; i<_nodes.size() ; i++ )
            if( _nodes[i].isEmpty() == false )
            {
                if( _nodes[i].status.childs == 0 )
                {
                    //FLOWGUI_REPORT(UnconnectedNode());
                    //break;
                    continue;
                }
                iter.to = i;
                iter.count = _nodes[i].status.childs;
                for( Idx j=0 ; j<_nodes.size() ; j++ )
                    if( _nodes[j].isEmpty() == false && _nodes[j].linkTo == iter.to )
                    {
                        iter.from = j;
                        return;
                    }
            }
        iter.reset();
        return;
    }
    //iterate 'from'
    for( Idx i=iter.from+1 ; i<_nodes.size() ; i++ )
        if( _nodes[i].isEmpty() == false && _nodes[i].linkTo == iter.to )
        {
            iter.from = i;
            iter.count--;
            return;
        }
//        FLOWGUI_REPORT(IllFormedLink());
    iter.reset();
}
std::optional<std::tuple<const QPointF&,const QPointF&>>
    LinkImp::iterateCoords(PairIter &iter) const
{
    if( iter.isInvalid() )
        return {};
    const auto& from = _nodes[iter.from].coord;
    const auto& to   = _nodes[iter.to  ].coord;
    iterate(iter);
    return {{from,to}};
}
std::optional<std::tuple<const LinkImp::Node&,const LinkImp::Node&>>
    LinkImp::iterateNodes(PairIter &iter) const
{
    if( iter.isInvalid() )
        return {};
    const auto& from = _nodes[iter.from];
    const auto& to   = _nodes[iter.to  ];
    iterate(iter);
    return {{from,to}};
}
std::optional<std::tuple<LinkImp::Idx,LinkImp::Idx>>
    LinkImp::iterateIndex(PairIter &iter) const
{
    if( iter.isInvalid() )
        return {};
    auto from = iter.from;
    auto to   = iter.to;
    iterate(iter);
    return {{from,to}};
}
void LinkImp::resetIter(NodeIter &iter) const
{
    if( _size == 0 )
    {
        iter.reset();
        return;
    }
    iter.idx = _rootIdx;
}
void LinkImp::iterate(NodeIter &iter) const
{
    for( Idx i=iter.idx+1 ; i<_nodes.size() ; i++ )
        if( _nodes[i].isEmpty() == false )
        {
            iter.idx = i;
            return;
        }
    iter.reset();
}
std::optional<QPointF> LinkImp::iterateCoords(NodeIter &iter) const
{
    if( iter.isInvalid() )
        return {};
    auto idx = iter.idx;
    iterate(iter);
    return {_nodes[idx].coord};
}

//----------------------------------------------
//----------- static methods
//----------------------------------------------
void LinkImp::switchLinePath(LinePath &linkPath)
{
    switch( linkPath )
    {
    case LinePath::straight:
        linkPath = LinePath::verticalThenHorizontal;
        break;
    case LinePath::verticalThenHorizontal:
        linkPath = LinePath::horizontalThenVertical;
        break;
    case LinePath::horizontalThenVertical:
        linkPath = LinePath::straightThenOrthogonal;
        break;
    case LinePath::straightThenOrthogonal:
        linkPath = LinePath::orthogonalThenStraight;
        break;
    case LinePath::orthogonalThenStraight:
        linkPath = LinePath::straight;
        break;
    }
}

//----------------------------------------------
//----------- internal methods
//----------------------------------------------
void LinkImp::swapRoot(Idx target)
{

}
std::optional<QPointF> LinkImp::computeMidPoint(const QPointF& startPoint,
                                                const QPointF& endPoint,
                                                const LinePath& linePath) const noexcept
{
    QPointF midp;
    switch( linePath )
    {
        case LinePath::straight:
            return {};
        case LinePath::verticalThenHorizontal:
            midp = QPointF( startPoint.x() , endPoint.y() );
            break;
        case LinePath::horizontalThenVertical:
            midp = QPointF( endPoint.x() , startPoint.y() );
            break;
        case LinePath::straightThenOrthogonal:
            {
                auto diffp = startPoint-endPoint;
                auto diffx = std::abs(diffp.x());
                auto diffy = std::abs(diffp.y());
                if( diffx < diffy )
                {
                    if( endPoint.y() > startPoint.y() )
                        midp = QPointF( endPoint.x() , startPoint.y()+diffx);
                    else
                        midp = QPointF( endPoint.x() , startPoint.y()-diffx);
                }
                else
                {
                    if( endPoint.x() > startPoint.x() )
                        midp = QPointF( startPoint.x()+diffy , endPoint.y() );
                    else
                        midp = QPointF( startPoint.x()-diffy , endPoint.y() );
                }
            }
            break;
        case LinePath::orthogonalThenStraight:
            {
                auto diffp = startPoint-endPoint;
                auto diffx = std::abs(diffp.x());
                auto diffy = std::abs(diffp.y());
                if( diffx <= diffy )
                {
                    if( endPoint.y() < startPoint.y() )
                        midp = QPointF( startPoint.x() , endPoint.y()+diffx);
                    else
                        midp = QPointF( startPoint.x() , endPoint.y()-diffx);
                }
                else
                {
                    if( endPoint.x() < startPoint.x() )
                        midp = QPointF( endPoint.x()+diffy , startPoint.y() );
                    else
                        midp = QPointF( endPoint.x()-diffy , startPoint.y() );
                }
            }
            break;
    }
    return midp;
}


} // namespace FlowGUI
