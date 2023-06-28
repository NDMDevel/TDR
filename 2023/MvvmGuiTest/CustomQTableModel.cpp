#include "CustomQTableModel.h"

#ifdef __EXCLUDE
CustomQTableModel::CustomQTableModel(QObject * parent)
    : QAbstractTableModel{parent}
{}
int CustomQTableModel::rowCount(const QModelIndex &) const
{
    return m_mblock.count();
}
int CustomQTableModel::columnCount(const QModelIndex &) const
{
    return 2;
}
QVariant CustomQTableModel::data(const QModelIndex &index, int role) const
{
    if( role != Qt::DisplayRole && role != Qt::EditRole )
        return {};
    const auto & modelBlock = m_mblock[index.row()];
    switch( index.column() )
    {
        case 0: return modelBlock.name();
        case 1: return modelBlock.type();
//        case 2: return modelBlock.registrationNumber();
        default: return {};
    }
//    const auto & vehicle = m_data[index.row()];
//    switch( index.column() )
//    {
//        case 0: return vehicle.make();
//        case 1: return vehicle.model();
//        case 2: return vehicle.registrationNumber();
//        default: return {};
//    }
}
QVariant CustomQTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation != Qt::Horizontal || role != Qt::DisplayRole )
        return {};
    switch( section )
    {
        case 0: return "Name";
        case 1: return "Type";
//        case 2: return "Reg.#";
        default: return {};
    }
}
void CustomQTableModel::append(const Vehicle & vehicle)
{
    beginInsertRows({}, m_data.count(), m_data.count());
    m_data.append(vehicle);
    endInsertRows();
}
void CustomQTableModel::append(const ModelBlock &modelBlock)
{
    beginInsertRows({}, m_data.count(), m_data.count());
    m_mblock.append(modelBlock);
    endInsertRows();
}
bool CustomQTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true;
}
Qt::ItemFlags CustomQTableModel::flags(const QModelIndex &index) const
{
/*
        NoItemFlags = 0,
        ItemIsSelectable = 1,
        ItemIsEditable = 2,
        ItemIsDragEnabled = 4,
        ItemIsDropEnabled = 8,
        ItemIsUserCheckable = 16,
        ItemIsEnabled = 32,
        ItemIsAutoTristate = 64,
#if QT_DEPRECATED_SINCE(5, 6)
        ItemIsTristate = ItemIsAutoTristate,
#endif
        ItemNeverHasChildren = 128,
        ItemIsUserTristate = 256
*/
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;//Qt::ItemIsEditable;
}
#endif
