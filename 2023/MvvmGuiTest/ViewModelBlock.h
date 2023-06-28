#ifndef VIEWMODELBLOCK_H
#define VIEWMODELBLOCK_H

#include "binding.h"
#include "property.h"
#include <QString>
#include "ViewBlock.h"

class ViewModelBlock
{
public:
public://properties
    bindey::property<QString> name;
    bindey::property<QString> type;
private:
    using View = std::tuple<ViewBlock,std::vector<nod::connection>>;
    std::vector<std::shared_ptr<View>> _views;
};

#endif // VIEWMODELBLOCK_H
