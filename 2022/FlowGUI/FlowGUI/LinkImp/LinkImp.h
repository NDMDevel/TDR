#ifndef FLOWGUI_LINKIMP_H
#define FLOWGUI_LINKIMP_H

//FlowGUI
#include "LinkImp/NodeType.h"
#include "BlockImp/PortType.h"
#include "Exceptions.h"

//std
#include <vector>
#include <cstdint>
#include <optional>
#include <variant>
#include <tuple>
#include <cmath>

namespace FlowGUI {

class LinkImp
{
public:
    //using Coord = QPointF;
    using Port  = PortType;
    using Idx   = uint32_t;
    static constexpr Idx invalid_index = ~Idx(0);
    using Node = NodeType;
public: //exported types
    enum class LinePath : uint8_t
    {
        straight,               //direct line between start and end point
        verticalThenHorizontal,
        horizontalThenVertical,
        straightThenOrthogonal, //45 degs then horizontal or vertical
        orthogonalThenStraight  //horizontal or vertical then 45 degs
    };
    class IterInterface
    {
    public:
        virtual bool isInvalid() const = 0;
        virtual void reset()           = 0;
    };
    class PairIter : private IterInterface
    {
        friend LinkImp;
    private:
        Idx from  = invalid_index;
        Idx to    = invalid_index;
        Idx count = 0;
    private:
        bool isInvalid() const override
        {
            return (from == invalid_index) || (to == invalid_index);
        }
        void reset() override
        {
            to    = invalid_index;
            from  = invalid_index;
            count = 0;
        }
    public:
        PairIter(const LinkImp &link)
        {
            link.resetIter(*this);
        }
    };
    class NodeIter : private IterInterface
    {
        friend LinkImp;
    private:
        Idx idx = invalid_index;
    private:
        bool isInvalid() const override
        {
            return idx == invalid_index;
        }
        void reset() override
        {
            idx = invalid_index;
        }
    public:
        NodeIter(const LinkImp &link)
        {
            link.resetIter(*this);
        }
    };
    class MoveType
    {
        friend LinkImp;
        std::optional<std::tuple<Idx,Idx,const QPointF&>> _movePos;
    };
//    using MoveType = std::optional<std::tuple<Idx,Idx,const QPointF&>>;
public:
    LinkImp(const QPointF& pos = QPointF()) noexcept;
    //move _src
    MoveType prepareMoveTo(const QPointF& pos);
    void moveTo(const MoveType &movePos);
    //lineTo: line from _src to pos, after lineTo ends, _src will be
    //move at _end (this implies that multiple invocations to
    //lineTo will start a line at the end of the previous one)
    void lineTo(const QPointF& pos,Port *port = nullptr);
    void nextLine(const QPointF &pos,Port *port = nullptr);
    void removeLastLine();
    static void switchLinePath();
    Idx createNode(const QPointF& coord,const Idx& linkTo,Port* port = nullptr) noexcept;
    void removeNode(Idx idx) noexcept;
    //select node at pos, or nodes under selArea, the selected
    //flag node will be set
    bool select(const QPointF& pos);
    bool select(const std::vector<QPointF>& selArea);
    void clearSelection();
    void displaceSelection(const QPointF& delta);
    std::tuple<double,double,double> squareTaxiDistancePointTo(const QPointF& pos) const;
    double squareTaxiDistanceTo(const QPointF& pos) const;
    std::variant<Idx,std::tuple<Idx,Idx,double,double>>
        getNearestPoint(const QPointF &pos) const;
/*    std::optional<std::tuple<LinkImp::Idx,LinkImp::Idx>>
        belongsToLine(Idx idxP1,
                      Idx idxP2,
                      const QPointF &point) const noexcept;*/
    std::tuple<Idx,Idx> getGrabbedIndexs(const QPointF &pos) const;
    Idx  size() const;

    //Node& operator[](Idx pos);
    //iterator:
    void resetIter(PairIter &iter) const;
    void iterate(PairIter &iter) const;
    std::optional<std::tuple<const QPointF&,const QPointF&>>
         iterateCoords(PairIter &iter) const;
    std::optional<std::tuple<const Node&,const Node&>>
         iterateNodes(PairIter &iter) const;
    std::optional<std::tuple<Idx,Idx>>
         iterateIndex(PairIter &iter) const;
    void resetIter(NodeIter &iter) const;
    void iterate(NodeIter &iter) const;
    std::optional<QPointF> iterateCoords(NodeIter &iter) const;

public: //statics methods
    static void switchLinePath(LinePath &linkPath);


private: //internal methods (must not be used outside the class)

    //helper to joint
    void swapRoot(Idx target);
    std::optional<QPointF> computeMidPoint(const QPointF& startPoint,
                                           const QPointF& endPoint,
                                           const LinePath& linePath) const noexcept;

private:
    std::vector<Node> _nodes;
    Idx _size    = 0;
    Idx _rootIdx = invalid_index;
    Idx _srcIdx  = invalid_index;
    Idx _midIdx  = invalid_index;
    Idx _endIdx  = invalid_index;
    static LinePath _path;
#warning "@attr hide not being used in this implementation"
    constexpr static bool _hide    = false;
};

} // namespace FlowGUI


#endif // FLOWGUI_LINKIMP_H
