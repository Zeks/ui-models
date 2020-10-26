


#include "../include/AdaptingTableModel.h"
#include "../include/TableDataInterface.h"
#include "../include/AdaptingTableModelPrivate.h"

AdaptingTableModel::AdaptingTableModel(QObject * parent) : QAbstractTableModel(parent)

{
    // Bouml preserved body begin 0020EEAA
    d = new AdaptingTableModelPrivate(this);
    // Bouml preserved body end 0020EEAA
}

AdaptingTableModel::~AdaptingTableModel() 
{
    // Bouml preserved body begin 0020EF2A
   if(d)
    delete d;
    // Bouml preserved body end 0020EF2A
}

QVariant AdaptingTableModel::data(const QModelIndex & index, int role) const 
{
    // Bouml preserved body begin 0020EFAA

    if(!index.isValid())
        return QVariant();
    return d->interface->GetValue(index.row(),  index.column(), role);
    // Bouml preserved body end 0020EFAA
}

bool AdaptingTableModel::setData(const QModelIndex & index, const QVariant & value, int role) 
{
    // Bouml preserved body begin 0021172A

    if(!index.isValid())
        return false;
    d->interface->SetValue(index.row(),  index.column(), role, value);
    return true;
    // Bouml preserved body end 0021172A
}

int AdaptingTableModel::rowCount(const QModelIndex & index) const 
{
    // Bouml preserved body begin 0020F02A
    Q_UNUSED(index);

    return d->interface->rowCount();
    // Bouml preserved body end 0020F02A
}

int AdaptingTableModel::columnCount(const QModelIndex & index) const 
{
    // Bouml preserved body begin 0020F0AA
    Q_UNUSED(index);

    return d->interface->columnCount();
    // Bouml preserved body end 0020F0AA
}

void AdaptingTableModel::SetInterface(QSharedPointer<TableDataInterface> _interface) 
{
    // Bouml preserved body begin 00218B2A

    d->interface = _interface;
    TableDataInterface* interfacePointer = d->interface.data();
	disconnect(interfacePointer, SIGNAL(reloadData()), this, SLOT(OnReloadDataFromInterface()));
    disconnect(interfacePointer, SIGNAL(reloadDataForRow(int)), this, SLOT(OnReloadRowDataFromInterface(int)));
    connect(interfacePointer, SIGNAL(reloadData()), this, SLOT(OnReloadDataFromInterface()));
    connect(interfacePointer, SIGNAL(reloadDataForRow(int)), this, SLOT(OnReloadRowDataFromInterface(int)));
    // Bouml preserved body end 00218B2A
}

bool AdaptingTableModel::insertRow(int row, const QModelIndex & )
{
    // Bouml preserved body begin 00020DAB

    if(row > rowCount())
        return false;
    beginInsertRows(QModelIndex(), row, row);
    d->interface->insertRow(row);
    endInsertRows();
    return true;
    // Bouml preserved body end 00020DAB
}

QModelIndex AdaptingTableModel::index(int row, int column, const QModelIndex & parent) const 
{
    // Bouml preserved body begin 00213CAA

    if (!hasIndex(row, column, parent))
        return QModelIndex();
    return createIndex(row, column, const_cast<void*>(d->interface->InternalPointer(row)));
    // Bouml preserved body end 00213CAA
}

Qt::ItemFlags AdaptingTableModel::flags(const QModelIndex & index) const 
{
    // Bouml preserved body begin 002170AA
    if (!index.isValid())
        return 0;

    Qt::ItemFlags result = d->interface->flags(index);
    return result;
    // Bouml preserved body end 002170AA
}

QVariant AdaptingTableModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
    // Bouml preserved body begin 0021712A

    if(d->interface->rowCount() == 0 )
        return QVariant();
    if(section < 0 || section > d->interface->GetColumns().size())
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        //qDebug() << interface->GetColumns().at(section);
        return d->interface->GetColumns().at(section);
    }
    return QVariant();
    // Bouml preserved body end 0021712A
}

void AdaptingTableModel::ClearData() 
{
    // Bouml preserved body begin 000217AB
    auto rowCountValue = rowCount();
    if(rowCountValue != 0)
    {
        beginRemoveRows(QModelIndex(), 0, rowCountValue-1);
        d->interface->DropData();
        endRemoveRows();
    }
    // Bouml preserved body end 000217AB
}

void AdaptingTableModel::updateAll() 
{
    // Bouml preserved body begin 000218AB
    emit dataChanged(index(0, 0, QModelIndex()), index(rowCount(), columnCount(),QModelIndex()));
    // Bouml preserved body end 000218AB
}

int AdaptingTableModel::RowForValue(void* value) 
{
    // Bouml preserved body begin 0002A62B

    for(int i(0); i < rowCount(QModelIndex()); ++i)
    {
        if(d->interface->Equal(i, value))
            return i;
    }
    return -1;
    // Bouml preserved body end 0002A62B
}


void AdaptingTableModel::RemoveRow(const QModelIndex & index) 
{
    // Bouml preserved body begin 0002A6AB

    if(!index.isValid())
        return;
    beginRemoveRows(QModelIndex(), index.row(),index.row());
    d->interface->RemoveRow(index.row());
    endRemoveRows();
    // Bouml preserved body end 0002A6AB
}

void AdaptingTableModel::OnReloadDataFromInterface() 
{
    // Bouml preserved body begin 0021702A

    beginResetModel();
    endResetModel();

    if(d->interface->PreviousRowCount() > 0)
    {
        int removeLimit = d->interface->PreviousRowCount() - 1;
        beginRemoveRows(QModelIndex(), 0, removeLimit);
        endRemoveRows();
    }
    if(d->interface->rowCount() > 0)
    {
        beginInsertRows(QModelIndex(), 0, d->interface->rowCount());
        endInsertRows();
    }
    // Bouml preserved body end 0021702A
}

void AdaptingTableModel::OnReloadRowDataFromInterface(int row) 
{
    // Bouml preserved body begin 0002142B
    emit dataChanged(index(row, 0,QModelIndex()), index(row, columnCount(),QModelIndex()));
    // Bouml preserved body end 0002142B
}

