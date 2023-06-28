#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H

#include <QSize>
#include <QPoint>

#include "bindey/property.h"
//#include "NamedProperty.h"

class BlockModel
{
public:
    BlockModel(QSize size = QSize(100,100));

public: //props
    bindey::property<QSize> size;
public: //events
    nod::signal<void()> changed;
};

#endif // BLOCKMODEL_H
