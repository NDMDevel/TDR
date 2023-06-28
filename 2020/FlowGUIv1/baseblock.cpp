#include "baseblock.h"
#include <QWidget>
//#include <QPainterPathStroker>

BaseBlock::BaseBlock(const QString &name,
                     const QString &type,
                     uint32_t gridSize)
    : name(name),
      type(type),
      face_index{0,1,2,3},
      gridSize(gridSize)
{
    updateShape();
    setFlag(QGraphicsItem::ItemIsMovable);
}

void BaseBlock::addConnector(BaseBlock::BlockFace face, BlockConnector &&connector)
{
    faceConnectors[face_index[static_cast<uint8_t>(face)]].push_back(connector);
}

uint32_t BaseBlock::getConnectorCount(BaseBlock::BlockFace face) const
{
    return faceConnectors[face_index[static_cast<uint8_t>(face)]].size();
}

void BaseBlock::rotateClockWise()
{
    uint8_t face_index_copy[faceCount];
    for( int i=faceCount-2 ; i>=0 ; i-- )
        face_index_copy[i] = face_index[i+1];
    face_index_copy[faceCount-1] = face_index[0];
    for( int i=0 ; i<faceCount ; i++ )
        face_index[i] = face_index_copy[i];

    qDebug()<< face_index[0] << " "
            << face_index[1] << " "
            << face_index[2] << " "
            << face_index[3];
    updateShape();
}

void BaseBlock::rotateCounterClockWise()
{
    uint8_t face_index_copy[faceCount];
    for( int i=0 ; i<faceCount-1 ; i++ )
        face_index_copy[i+1] = face_index[i];
    face_index_copy[0] = face_index[faceCount-1];
    for( int i=0 ; i<faceCount ; i++ )
        face_index[i] = face_index_copy[i];

    qDebug()<< face_index[0] << " "
            << face_index[1] << " "
            << face_index[2] << " "
            << face_index[3];
    updateShape();
}

void BaseBlock::flipHorizontal()
{
    auto aux = face_index[0];
    face_index[0] = face_index[2];
    face_index[2] = aux;
    qDebug()<< face_index[0] << " "
            << face_index[1] << " "
            << face_index[2] << " "
            << face_index[3];
    updateShape();
}

void BaseBlock::flipVertical()
{
    auto aux = face_index[1];
    face_index[1] = face_index[3];
    face_index[3] = aux;
    qDebug()<< face_index[0] << " "
            << face_index[1] << " "
            << face_index[2] << " "
            << face_index[3];
    updateShape();
}

/**
 * @brief Crea/Actualiza forma del bloque
 *
 * Si la cantidad de IO, textos o direccion
 * cambian, es necesario llamar este metodo
 * para actualizar los cambios de forma o
 * crearla si el bloque esta recien creado.
 */
void BaseBlock::updateShape()
{
    path.clear();
    QSizeF size = findMinBlockSize();
    size.setWidth(size.width()+gridSize*0);
    size.setHeight(size.height()+gridSize*0);
    boundRect.setRect(0*gridSize/2,0*gridSize/2,size.width(),size.height());
    drawBlock();
    update();
//    boundRect.setSize(size);

//    drawFace(BlockFace::Left/*,path*/);
//    drawFace(BlockFace::Top/*,path*/);
//    path.addRect(boundRect);


//    path.setFillRule(Qt::WindingFill);    //uncomment this!!

/*
    QPainterPath con;
    con.addRect(gridSize/4,gridSize/4+gridSize,gridSize/2,gridSize/2);
    QRectF rect(gridSize/2,gridSize/2,size.width()-gridSize,size.height()-gridSize);
    path.addRect(rect);
    path.addPath(con);
*/
}

void BaseBlock::contextMenuExec(const QPoint &p)
{
    menu.exec(p);
}

void BaseBlock::contextMenuAddAction(const QString &text,
                                     const QObject *receiver,
                                     const char *member)
{
    menu.addAction(text,receiver,member);
}

void BaseBlock::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    //just to avoid unsed vars warnings
    if( widget == nullptr || option == nullptr ){}

    painter->setBrush(Qt::white);
    painter->setPen(Qt::black);
    painter->drawPath(path);
}

QSizeF BaseBlock::findMinBlockSize()
{
    QSizeF size;
    uint32_t hl = ( 2 + faceConnectors[face_index[static_cast<uint8_t>(BlockFace::First)]].size()  ) * gridSize;
    uint32_t hr = ( 2 + faceConnectors[face_index[static_cast<uint8_t>(BlockFace::Third)]].size() ) * gridSize;
    uint32_t wb = ( 2 + faceConnectors[face_index[static_cast<uint8_t>(BlockFace::Second)]].size()  ) * gridSize;
    uint32_t wt = ( 2 + faceConnectors[face_index[static_cast<uint8_t>(BlockFace::Four)]].size()   ) * gridSize;

    if( hl != gridSize && (wb != gridSize || wt != gridSize) )
    {
        hl += gridSize;
        if( wb != gridSize || wt != gridSize )
            hl += gridSize;
    }
    if( hr != gridSize && (wb != gridSize || wt != gridSize) )
    {
        hr += gridSize;
        if( wb != gridSize || wt != gridSize )
            hr += gridSize;
    }
    if( wt != gridSize && (hl != gridSize || hr != gridSize) )
    {
        wt += gridSize;
        if( hl != gridSize || hr != gridSize )
            wt += gridSize;
    }
    if( wb != gridSize && (hl != gridSize || hr != gridSize) )
    {
        wb += gridSize;
        if( hl != gridSize || hr != gridSize )
            wb += gridSize;
    }

    size.setHeight(hr);
    size.setWidth(wt);
    if( hl > hr )
        size.setHeight(hl);
    if( wb > wt )
        size.setWidth(wb);

    return size;
}

void BaseBlock::drawBlock()
{
    path.addRect(boundRect);
    path.addRect(gridSize,
                 gridSize,
                 boundRect.width()-gridSize*2,
                 boundRect.height()-gridSize*2);
    for( int face_idx=0 ; face_idx<faceCount ; face_idx++ )
    {
        const auto &con_array = faceConnectors[face_index[face_idx]];
        const uint32_t con_len = con_array.size();
        if( con_len == 0 )
            continue;
        for( uint8_t con_idx=0 ; con_idx<con_len ; con_idx++ )
            addConnectorShape(static_cast<BlockFace>(face_idx),con_idx);
    }
}

void BaseBlock::addConnectorShape(BaseBlock::BlockFace face, uint8_t con_idx)
{
//    const bool shift_vert  = faceConnectors[face_index[static_cast<uint8_t>(BlockFace::Four)]].size()   != 0 ||
//                             faceConnectors[face_index[static_cast<uint8_t>(BlockFace::Second)]].size() != 0;
//    const bool shift_horz  = faceConnectors[face_index[static_cast<uint8_t>(BlockFace::First)]].size()  != 0 ||
//                             faceConnectors[face_index[static_cast<uint8_t>(BlockFace::Third)]].size()  != 0;
    bool shift_vert = true;
    bool shift_horz = true;
    switch( face )
    {
        case BlockFace::First:
            if( !shift_vert )
            {
                path.moveTo(gridSize,(con_idx+1)*gridSize);
                path.lineTo(gridSize-gridSize/2,(con_idx+1)*gridSize-gridSize/3);
                path.moveTo(gridSize,(con_idx+1)*gridSize);
                path.lineTo(gridSize-gridSize/2,(con_idx+1)*gridSize+gridSize/3);
            }
            else
            {
                path.moveTo(gridSize,(con_idx+2)*gridSize);
                path.lineTo(gridSize-gridSize/2,(con_idx+2)*gridSize-gridSize/3);
                path.moveTo(gridSize,(con_idx+2)*gridSize);
                path.lineTo(gridSize-gridSize/2,(con_idx+2)*gridSize+gridSize/3);
            }
            break;
/*
            if( !shift_vert )
                path.addRect(gridSize-gridSize/4,(con_idx+1)*gridSize-gridSize/4,gridSize/2,gridSize/2);
            else
                path.addRect(gridSize-gridSize/4,(con_idx+2)*gridSize-gridSize/4,gridSize/2,gridSize/2);
            break;*/
        case BlockFace::Second:
//            if( con_idx != 0 )
//                break;
            if( !shift_vert )
            {
                path.moveTo(gridSize,(con_idx+1)*gridSize);
                path.lineTo(gridSize-gridSize/2,(con_idx+1)*gridSize-gridSize/3);
                path.moveTo(gridSize,(con_idx+1)*gridSize);
                path.lineTo(gridSize-gridSize/2,(con_idx+1)*gridSize+gridSize/3);
            }
            else
            {
                path.moveTo((con_idx+2)*gridSize,boundRect.height()-gridSize);
                path.lineTo((con_idx+2)*gridSize-gridSize/3,boundRect.height()-gridSize/2);
                path.moveTo((con_idx+2)*gridSize,boundRect.height()-gridSize);
                path.lineTo((con_idx+2)*gridSize+gridSize/3,boundRect.height()-gridSize/2);
//                path.lineTo((con_idx+2)*gridSize-gridSize/3,boundRect.height()+gridSize/2);
                //path.moveTo(gridSize,(con_idx+2)*gridSize);
                //path.lineTo(gridSize-gridSize/2,(con_idx+2)*gridSize+gridSize/3);
            }
            break;
/*
            if( !shift_horz )
                path.addRect((con_idx+1)*gridSize-gridSize/4,boundRect.height()-gridSize/4,gridSize/2,gridSize/2);
            else
                path.addRect((con_idx+2)*gridSize-gridSize/4,boundRect.height()-gridSize/4,gridSize/2,gridSize/2);
            break;*/
        case BlockFace::Third:
            if( !shift_vert )
                path.addRect(boundRect.width()-gridSize/4,(con_idx+1)*gridSize-gridSize/4,gridSize/2,gridSize/2);
            else
                path.addRect(boundRect.width()-gridSize/4,(con_idx+2)*gridSize-gridSize/4,gridSize/2,gridSize/2);
            break;
        case BlockFace::Four:
            if( !shift_horz )
                path.addRect((con_idx+1)*gridSize-gridSize/4,gridSize-gridSize/4,gridSize/2,gridSize/2);
            else
                path.addRect((con_idx+2)*gridSize-gridSize/4,gridSize-gridSize/4,gridSize/2,gridSize/2);
            break;
    }
}
