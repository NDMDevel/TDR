#ifndef CGRAPHICSVIEW_H
#define CGRAPHICSVIEW_H

#include <QWheelEvent>
#include <QGraphicsView>
#include <QDebug>

class CGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    CGraphicsView(QWidget *parent) : QGraphicsView(parent)
    {
        setRenderHints(QPainter::Antialiasing |
                       QPainter::SmoothPixmapTransform |
                       QPainter::HighQualityAntialiasing);
    }
signals:
    void StartLine(const QPoint& point);
    void UpdateEndLine(const QPoint& point);
    void MouseRelease(const QPoint& point);
protected slots:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    bool mousePress = false;
};

#endif // CGRAPHICSVIEW_H
