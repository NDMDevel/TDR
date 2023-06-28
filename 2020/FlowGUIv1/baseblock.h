#ifndef BASEBLOCK_H
#define BASEBLOCK_H

#include <QGraphicsItem>
#include <QPainter>
#include <QMenu>
#include <QDebug>
#include "blockconnector.h"
#include <vector>

class BaseBlock : public QGraphicsItem
{
public:
    enum class BlockFace        : uint8_t { First=0,Second,Third,Four };
protected:
    QString name;
    QString type;

    QPainterPath path;
    QMenu menu;
    QBrush brush;
    QPen pen;

    static constexpr uint8_t faceCount = 4;
    std::vector<BlockConnector> faceConnectors[faceCount];
    uint8_t face_index[faceCount];
    uint32_t gridSize;
    QRectF  boundRect;
public:
    BaseBlock(const QString &name,
              const QString &type,
              uint32_t gridSize);

    //block config
    void     addConnector(BlockFace face,BlockConnector &&connector);
    uint32_t getConnectorCount(BlockFace face) const;

    //geometry modifiers
    void setGridSize(uint32_t size){ gridSize = size; }
    void rotateClockWise();
    void rotateCounterClockWise();
    void flipHorizontal();
    void flipVertical();
    uint32_t getGridSize() const { return gridSize; }

    void updateShape();

    //general getters & setters:
    const QString& getName() const { return name; }
    const QString& getType() const { return type; }

    void contextMenuExec(const QPoint &p);
    void contextMenuAddAction(const QString &text, const QObject *receiver, const char* member);

    QRectF boundingRect() const override { return boundRect; }
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
private:
    QSizeF findMinBlockSize();
    void drawFace(BlockFace face,QPainterPath &path);

    //internal support methods
    void drawBlock();
    void setFaceColor();
    void addConnectorShape(BlockFace face,uint8_t con_idx);
    enum class ConnectorShape { Triangle,Square,HalfCircle };
    void makeConnectorShape(double x,double y,ConnectorShape);
};

#endif // BASEBLOCK_H
