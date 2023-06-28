#ifndef FLOWGUI_LINKCONTAINERINTERFACE_H
#define FLOWGUI_LINKCONTAINERINTERFACE_H

//FlowGUI

//std
#include <cstdint>
#include <optional>
#include <tuple>

//Qt

namespace FlowGUI {

template<typename Idx,typename Coord>
class LinkContainerInterface
{
public: //exported types
    struct Iter
    {
        Idx from;
        Idx to;
    };
public:
    /** All methods that returns an index type (idx_t)
     * should never throw, instead return invalid_index
     */
    virtual Idx  insert(const Coord &coord) noexcept = 0;
    virtual Idx  append(Idx parentIdx,const Coord &coord) noexcept = 0;
    virtual Idx  insertBefore(Idx targetIdx,const Coord &coord) noexcept = 0;
    virtual void removeSubTree(Idx targetIdx) noexcept = 0;

    virtual std::optional<Coord >
        operator[](Idx targetIdx) noexcept = 0;

    virtual void resetIter(Iter &iter) noexcept = 0;

    virtual std::optional<Coord>
        iterate(Iter &iter) const noexcept = 0;

    virtual std::optional<std::tuple<Coord,Coord>>
        iterateLine(Iter &iter) const noexcept = 0;

    virtual std::optional<std::tuple<const Idx&,const Idx&>>
        iterateLineIdx(Iter &iter) const noexcept = 0;
};

} // namespace FlowGUI

#endif // FLOWGUI_LINKCONTAINERINTERFACE_H
