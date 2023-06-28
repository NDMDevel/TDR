#include "cgraphicsview.h"


void CGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        // zoom
        const ViewportAnchor anchor = transformationAnchor();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        int angle = event->angleDelta().y();
        qreal factor;
        if (angle > 0) {
            factor = 1.1;
        } else {
            factor = 0.9;
        }
        scale(factor, factor);
        setTransformationAnchor(anchor);
    }
    else
        QGraphicsView::wheelEvent(event);

    qreal deltaScale = 1;
    deltaScale += event->delta() > 0 ? 0.05 : -0.05;
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scale(deltaScale, deltaScale);
}

void CGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if( itemAt(event->pos()) == nullptr )
    {
        mousePress = true;
        emit StartLine(mapTo(this,event->pos()));
    }
    QGraphicsView::mousePressEvent(event);
}

void CGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if( mousePress )
    {
        emit MouseRelease(mapTo(this,event->pos()));
        mousePress = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void CGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if( mousePress )
    {
        static int i=0;
        if( items(event->pos()).length() > 1 )
            qDebug() << items(event->pos()).takeAt(0) <<
                        items(event->pos()).takeAt(1) <<
                        i++;
        else
            if( items(event->pos()).length() > 0 )
                qDebug() << items(event->pos()).takeAt(0) <<
                            i++;

        emit UpdateEndLine(mapTo(this,event->pos()));
    }
    QGraphicsView::mouseMoveEvent(event);
}
