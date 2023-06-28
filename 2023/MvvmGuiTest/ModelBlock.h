#ifndef MODELBLOCK_H
#define MODELBLOCK_H

#include <QString>

class ModelBlock
{
public:
    ModelBlock()
        : name("Default name")
        , type("Def type"){}
public://properties
    //bindey::property<QString> name;
    //bindey::property<QString> type;
    QString name;
    QString type;
};

#endif // MODELBLOCK_H
