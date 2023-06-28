#include "View.h"
//FlowGUI
#include "Style.h"
#include "Utils.h"
#include "Exceptions.h"

//std
#include <cmath>

//Qt
#include <QMouseEvent>

namespace FlowGUI {

View::View(QWidget *parent) : QGraphicsView(parent),_scene(parent)
{
    setOptimizationFlag(QGraphicsView::DontSavePainterState);
    setRenderHints(QPainter::Antialiasing |
                   QPainter::SmoothPixmapTransform);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setCacheMode(QGraphicsView::CacheBackground);

//    setResizeAnchor(ViewportAnchor::NoAnchor);
//    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
//    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    //setSceneRect();

    QBrush brush;
    brush.setColor(StyleGrid::backgroundGridColor);
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    setBackgroundBrush(brush);

    //call setItemIndexMethod with NoIndex is necessary to void crashes
    //when deleting (manually) the links removed from the scene.
    //It seems that the scene keeps reference to the links
    //after removing it in the BSPItemIndexMethod
    _scene.setItemIndexMethod(QGraphicsScene::ItemIndexMethod::NoIndex);
    QGraphicsView::setScene(&_scene);

}
void View::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    auto pos = event->pos();
    auto scenePos = mapToScene(pos);
    emit mouseMoved(pos,scenePos);
}
void View::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}
void View::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}
void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
}
void View::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
}
void View::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyReleaseEvent(event);
}
void View::drawBackground(QPainter* painter, const QRectF &r)
{
    QGraphicsView::drawBackground( painter , r );

    auto drawGrid = [&](double gridStep)
    {
        QRect   windowRect = rect();
        QPointF tl = mapToScene(windowRect.topLeft());
        QPointF br = mapToScene(windowRect.bottomRight());

        double left   = std::floor(tl.x() / gridStep - 0.5);
        double right  = std::floor(br.x() / gridStep + 1.0);
        double bottom = std::floor(tl.y() / gridStep - 0.5);
        double top    = std::floor (br.y() / gridStep + 1.0);

        // vertical lines
        for (int xi = int(left); xi <= int(right); ++xi)
        {
            QLineF line(xi * gridStep, bottom * gridStep,
                        xi * gridStep, top * gridStep );

            painter->drawLine(line);
        }

        // horizontal lines
        for (int yi = int(bottom); yi <= int(top); ++yi)
        {
            QLineF line(left * gridStep, yi * gridStep,
                        right * gridStep, yi * gridStep );
            painter->drawLine(line);
        }
    };

    //  auto const &flowViewStyle = StyleCollection::flowViewStyle();

    QPen pfine(StyleGrid::fineGridColor,
               qreal(StyleGrid::fineGridWidth));

    painter->setPen(pfine);
    drawGrid(qreal(StyleGrid::gridSize));

    QPen p(StyleGrid::coarseGridColor,
           qreal(StyleGrid::coarseGridWidth));

    painter->setPen(p);
    drawGrid(10.0*qreal(StyleGrid::gridSize));
}
} // namespace FlowGUI
