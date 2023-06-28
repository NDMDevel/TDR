#include "blockviewmodel.h"
#include <QDebug>

BlockViewModel::BlockViewModel(BlockModel& model)
    : _model(model)
{
    //two-way binding
    bindey::bind(_model.size,_size);
    bindey::bind(_size,_model.size);

    _model.changed.connect([&](){ changedSignal(); });
}
void BlockViewModel::setSize(QSize size)
{
    _size.set(size);
}
void BlockViewModel::onClick(QPoint pos,MouseEventMetadata meta)
{
    /*if( meta.btn == MouseEventMetadata::Button::left )
        qDebug() << "L:" << pos;
    if( meta.btn == MouseEventMetadata::Button::right )
        qDebug() << "R:" << pos;
    if( meta.btn == MouseEventMetadata::Button::middle )
        qDebug() << "M:" << pos;
    if( meta.altKey )
        qDebug() << "alt";
    if( meta.ctrlKey )
        qDebug() << "ctrl";
    if( meta.shiftKey )
        qDebug() << "shift";*/
}
void BlockViewModel::onMouseMove(QPoint pos, MouseEventMetadata meta)
{
    if( meta.alt )
        qDebug() << "alt";
    if( meta.ctrl )
        qDebug() << "ctrl";
    if( meta.shift )
        qDebug() << "shift";
    if( meta.shift )
    {
        _markPoint.set(pos);
        _drawMark.set(meta.ctrl);
        if( _drawMark.get() )
            changedSignal();
    }

}
