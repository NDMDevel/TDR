//FlowGUI
#include "Vector.h"

//std

//Qt

namespace FlowGUI {

Vector::Vector(const Coord &start) noexcept
{
    _nodes.reserve(3);
    _nodes.push_back(start);
    _size = 1;
    _srcIdx = 0;
}

bool Vector::moveTo(Idx to)
{

}

void Vector::lineTo(const Coord &to)
{

}

Vector::Node& Vector::operator[](Idx pos)
{
    return _nodes[pos];
}

} // namespace FlowGUI
