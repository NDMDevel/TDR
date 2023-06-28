#ifndef CUSTOMQTABLEMODEL_H
#define CUSTOMQTABLEMODEL_H

#include <QAbstractTableModel>
#include <QTableView>
#include "ModelBlock.h"
/*
class Vehicle {
   QString m_make, m_model, m_registrationNumber;
public:
   Vehicle(const QString & make, const QString & model, const QString & registrationNumber) :
      m_make{make}, m_model{model}, m_registrationNumber{registrationNumber} {}
   QString make() const { return m_make; }
   QString model() const { return m_model; }
   QString registrationNumber() const { return m_registrationNumber; }
   bool isRegistered() const { return !m_registrationNumber.isEmpty(); }
};

class CustomQTableModel : public QAbstractTableModel
{
public:
    CustomQTableModel(QObject * parent = {});
    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void append(const Vehicle & vehicle);
    void append(const ModelBlock & modelBlock);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
private:
    QList<Vehicle> m_data;
    QList<ModelBlock> m_mblock;
};
*/
#endif // CUSTOMQTABLEMODEL_H
