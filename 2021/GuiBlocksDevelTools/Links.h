#ifndef GUIBLOCKS_LINKS_H
#define GUIBLOCKS_LINKS_H

#include <QGraphicsItem>
#include <vector>

#include "Node.h"

namespace GuiBlocks {

class Links : public QGraphicsItem
{
public:
    Links();
    //virtual ~Links()override{}

    //pure virtual required
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

private:
    std::vector<Node> nodes;
};

} // namespace GuiBlocks

//tmin    = zeros(1,1);
//Idc_min = zeros(1,1);
//Idc_avg = zeros(1,1);
//Idc_max = zeros(1,1);
//Vdc_min = zeros(1,1);
//Vdc_avg = zeros(1,1);
//Vdc_max = zeros(1,1);
//Ws_min  = zeros(1,1);
//Ws_avg  = zeros(1,1);
//Ws_max  = zeros(1,1);
//Pwr_min = zeros(1,1);
//Pwr_avg = zeros(1,1);
//Pwr_max = zeros(1,1);

#endif // GUIBLOCKS_LINKS_H
