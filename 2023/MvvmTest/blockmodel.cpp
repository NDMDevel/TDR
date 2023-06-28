#include "blockmodel.h"

BlockModel::BlockModel(QSize size)
{
    this->size.set(size);
    this->size.onChanged([&](auto){ changed(); });
}
