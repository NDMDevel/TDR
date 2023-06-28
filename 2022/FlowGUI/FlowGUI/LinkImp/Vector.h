#ifndef FLOWGUI_VECTOR_H
#define FLOWGUI_VECTOR_H

//FlowGUI
#include "LinkImp/NodeType.h"
#include "BlockImp/PortInterface.h"
#include "Link.h"

//std
#include <cstdint>
#include <vector>

//Qt
#include <QPointF>

namespace FlowGUI {

class Vector //: public LinkContainerInterface<Idx,Coord>
{
public: //exported types & constants
    using Idx   = uint16_t;
    using Coord = QPointF;
    using Port  = PortInterface;
    static constexpr Idx invalid_index = ~Idx(0); //all bits high
    using Node = NodeType<Coord,Port,Idx,invalid_index>;

public:
    Vector(const Coord& startCoord) noexcept;

    bool moveTo(Idx to);
    void lineTo(const Coord& to);
    bool changeAt(Idx at,const Coord& val);

    Idx  size() const { return _size; }
    Node& operator[](Idx pos);

private: //internal methods (must not be used outside the class)

    //helper to joint
    void swapRoot(Idx target);

private:
    std::vector<Node> _nodes;
    Idx _size    = 0;
    Idx _rootIdx = invalid_index;
    Idx _srcIdx  = invalid_index;
};

} // namespace FlowGUI

#endif // FLOWGUI_VECTOR_H
