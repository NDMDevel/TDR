#ifndef GUIBLOCKS_NODE_H
#define GUIBLOCKS_NODE_H

//std
#include <cstdint>

//Qt
#include <QPointF>

//GuiBlocks
#include "GuiBlocks/Block.h"
#include "GuiBlocks/Utils.h"

namespace GuiBlocks {

class Links;
class Node
{
public: //exported types
    enum class Attributes : uint32_t
    {
        #define BIT(pos) uint32_t(1)<<pos

        hover    = BIT(0),
        selected = BIT(1),

        empty    = BIT(31),
        clear    = 0
    };
public:
    //ctors (rule of 5 needed? there is no resource asociated with node)
    Node();
    ~Node();
    Node(const QPointF &p,
         uint32_t id,
         uint32_t parent    = idx32::invalid_index,
         Block::Port *bport = nullptr);
    Node(Node &&n) noexcept;
    Node &operator=(const Node &n);
    Node &operator=(Node &&n) noexcept;

    //general methods
    operator bool()const;   //true if node is not empty (is in valid state)
    bool isEmpty() const;
    void makeEmpty();
    uint32_t getLinkId() const;
    QPointF  getPoint() const;
    void     setPoint(const QPointF &p);

private:
    void releaseResource();

private:
    QPointF     point;
    uint32_t    parentIdx;
    Attributes  attr;
    uint32_t    linkId;
    Block::Port *port;
};


} // namespace GuiBlocks

#endif // GUIBLOCKS_NODE_H
