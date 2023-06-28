#include "blockconnector.h"

BlockConnector::BlockConnector(Direction dir,
                               Type type,
                               const QString &name,
                               State state)
    : dir(dir),
      type(type),
      state(state),
      name(name)
{
}
