#ifndef VIEWBLOCK_H
#define VIEWBLOCK_H

#include <QGraphicsItem>
#include "property.h"

class ViewModelBlock;

class ViewBlock : public QGraphicsItem
{
private:
    ViewModelBlock& _viewModel;
public:
    ViewBlock(ViewModelBlock& viewModel);

    QRectF boundingRect() const override;
    void   paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

public: //properties
    QString name;
    QString type;
};

#endif // VIEWBLOCK_H
