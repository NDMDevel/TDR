#include "Style.h"
#include <QColor>
#include <QSize>

namespace FlowGUI {

double StyleText::gapTypeToBorderGridSizePercent = 0.1;
double StyleText::gapTextToBorderGridSizePercent = 0.1;
QFont  StyleText::blockTypeFont = QFont("Tahoma", 11);
QColor StyleText::blockTypeColor = Qt::red;
QFont  StyleText::blockNameFont = QFont("Tahoma", 11);
QColor StyleText::blockNameColor = Qt::blue;
QFont  StyleText::blockHintFont = QFont("Tahoma", 11);
QColor StyleText::blockHintColor = Qt::black;

QSizeF StyleBlockShape::connectorSizeGridSizePercent    = QSizeF( 0.5 , 0.6 );
QColor StyleBlockShape::blockRectBorderColor            = "#202020";//Qt::gray;
QColor StyleBlockShape::blockRectBorderColorOnHover     = Qt::magenta;//Qt::black;
QColor StyleBlockShape::blockRectBorderColorIfSelected  = Qt::blue;
QColor StyleBlockShape::shadowColor                     = "#202020";//Qt::gray;
QColor StyleBlockShape::shadowColorOnHover              = Qt::black;
QLinearGradient StyleBlockShape::customFillGradient     = QLinearGradient(0.5,0,0.5,1);
QColor StyleBlockShape::blockRectFillColor1             = Qt::white;
QColor StyleBlockShape::blockRectFillColor2             = "#CECECE";
double StyleBlockShape::roundingXWidthPercent           = 0.02;
double StyleBlockShape::roundingYWidthPercent           = 0.02;
double StyleBlockShape::opacity                         = 1.0;
double StyleBlockShape::fillOpacity                     = 1.0;
QColor StyleBlockShape::inputConnectorBorderColor       = Qt::black;
QColor StyleBlockShape::outputConnectorBorderColor      = Qt::black;
QColor StyleBlockShape::inputConnectorFillColor         = Qt::green;
QColor StyleBlockShape::outputConnectorFillColor        = Qt::red;

double StyleGrid::gridSize = 20.0;
QColor StyleGrid::backgroundGridColor = Qt::white;
double StyleGrid::fineGridWidth       = 1.0;
QColor StyleGrid::fineGridColor       = Qt::lightGray;
double StyleGrid::coarseGridWidth     = 1.1;
QColor StyleGrid::coarseGridColor     = Qt::gray;

QColor StyleLink::lineColor          = Qt::blue;
QColor StyleLink::nodeColor          = Qt::magenta;
double StyleLink::width              = 2.0;
QColor StyleLink::shadowColor        = "#202020";
Qt::PenStyle    StyleLink::normalLine = Qt::SolidLine;
Qt::PenCapStyle StyleLink::normalCap  = Qt::RoundCap;

QColor StyleSelection::normalFillColor  = Qt::blue;
QColor StyleSelection::cuttedFillColor  = "#E08000";
//QColor StyleSelection::normalLineColor  = Qt::black;
//QColor StyleSelection::cuttedLineColor  = Qt::red;
double StyleSelection::width        = 2.0;
double StyleSelection::opacity      = 0.1;
Qt::PenStyle    StyleSelection::normalLine = Qt::PenStyle::SolidLine;
Qt::BrushStyle  StyleSelection::fillStyle  = Qt::BrushStyle::SolidPattern;

//QString LogFilePath::path = "../";

} // namespace FlowGUI
