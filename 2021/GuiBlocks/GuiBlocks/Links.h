#ifndef GUIBLOCKS_LINKS_H
#define GUIBLOCKS_LINKS_H

//std
#include <vector>
#include <cstdint>
#include <optional>

//Qt
#include <QGraphicsItem>
#include <QPainter>

//GuiBlocks
#include "GuiBlocks/Node.h"
#include "GuiBlocks/Block.h"
#include "GuiBlocks/Utils.h"

namespace GuiBlocks {

class Links : public QGraphicsItem
{
public: //exported types
    enum class LinkPath : uint8_t
    {
        straight,               //direct line between start and end point
        verticalThenHorizontal,
        horizontalThenVertical,
        straightThenOrthogonal, //45 degs then horizontal or vertical
        orthogonalThenStraight  //horizontal or vertical then 45 degs
    };
    struct IterHandler  //to be use with method iterate()
    {
        uint32_t from = idx32::invalid_index;
        uint32_t to   = idx32::invalid_index;
    };
private: //internal types

public: //public interface
    Links();
    void appendRandomLine();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

    //line management methods:
    void startLineAt(const QPointF &start,const QPointF &end,LinkPath path,Block::Port *connPort=nullptr);
    void updateLastLine(const QPointF &end,LinkPath path);
    void removeLastLine();
    uint32_t size() const;

    //iterators
    void        resetIter(IterHandler &iter) const noexcept;
    IterHandler resetIter() const noexcept;
    std::optional<std::tuple<uint32_t,uint32_t>> iterate(IterHandler &iter) const noexcept;

private: //internal vars
    std::vector<Node> nodes;
    uint32_t startIdx,midIdx,endIdx;
    QRectF _boundingRect;
    uint32_t _size;

private: //internal methods
    uint32_t getUnusedLinkId() const;
    void updateBoundingRect();
    std::optional<QPointF> computeMidPoint(const QPointF  &startPoint,
                                           const QPointF  &endPoint,
                                           const LinkPath &linkPath) const noexcept;

    //this method will append a node in the free space of "nodes", and if
    //there is no available space, nodes will be expanded
    uint32_t appendNode(Node &&n) noexcept;
    uint32_t getNextEmptyNode(uint32_t startPos = 0) const noexcept;
    uint32_t getNextNonEmptyNode(uint32_t startPos = 0) const noexcept;
};

} // namespace GuiBlocks

#endif // GUIBLOCKS_LINKS_H
