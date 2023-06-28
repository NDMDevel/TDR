#ifndef FLOWGUI_LINKTREEARRAY_H
#define FLOWGUI_LINKTREEARRAY_H

//FlowGUI
#include "LinkImp/LinkContainerInterface.h"
#include "LinkImp/NodeTypeIll.h"
#include "BlockImp/PortType.h"

//std
#include <vector>

//Qt
#include <QPointF>

#warning "Dont use this implementation, is ill-formed"

namespace FlowGUI {

using Idx   = uint16_t;
using Coord = QPointF;
using PortType  = PortType;

class LinkTreeArray : public LinkContainerInterface<Idx,Coord>
{
public: //exported types
    static constexpr Idx invalid_index = ~Idx(0); //all bits high

private://internal types
    using node_t = NodeTypeIll<Idx,Coord,PortType,invalid_index>;

public:
    LinkTreeArray();

public:
    Idx   insert(const Coord &coord) noexcept override;
    Idx   append(Idx parentIdx,const Coord &coord)       noexcept override;
    //pending
    Idx   insertBefore(Idx targetIdx,const Coord &coord) noexcept override;
    //pending
    void    removeSubTree(Idx targetIdx) noexcept override;
    //pending
    std::optional<Coord> operator[](Idx targetIdx) noexcept override;

    void    resetIter(Iter &iter)  noexcept override;
    //pending
    virtual std::optional<Coord>
        iterate(Iter &iter) const noexcept override;
    //pending
    virtual std::optional<std::tuple<Coord,Coord>>
        iterateLine(Iter &iter) const noexcept override;

    virtual std::optional<std::tuple<const Idx&,const Idx&>>
        iterateLineIdx(Iter &iter) const noexcept override;

    bool  isEmpty() const { return size == 0; }
    Idx createNode(node_t &&node) noexcept;

    auto length() const {return size;}

private:
    std::vector<node_t> nodes;
    uint32_t size;
};

} // namespace FlowGUI

#endif // FLOWGUI_LINKTREEARRAY_H
