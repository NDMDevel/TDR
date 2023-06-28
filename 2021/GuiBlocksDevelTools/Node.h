#ifndef GUIBLOCKS_NODE_H
#define GUIBLOCKS_NODE_H

//Qt
#include <QPointF>

//std
#include <cstdint>

//GuiBlocks
namespace GuiBlocks {

class Node
{
public: //exported types
    enum class Attributes : uint32_t
    {
        #define BIT(pos) uint32_t(1)<<pos

        hover    = BIT(0),
        selected = BIT(1),

        empty    = BIT(31)
    };
public:
    //ctors (rule of 5 needed? there is no resource asociated with node)
    Node();

    //general methods
    bool isEmpty()const{ return attr == Attributes::empty;}

private:
    QPointF    point;
    uint32_t   parentIdx;
    Attributes attr;
    void *port;
};

} // namespace GuiBlocks

#endif // GUIBLOCKS_NODE_H
