#include <QSize>
#ifdef GUI_APPLICATION
#include <QColor>
#include <QFont>
#endif
#include "include/TreeModel.h"
#include "include/ItemController.h"
#include "include/TreeItemInterface.h"
#ifdef DLL_PROJECT
QThreadStorage<int> FunctionTracer::logLevel;

#endif
TreeModel::TreeModel(QObject * parent)
    : QAbstractItemModel(parent)

{
    // Bouml preserved body begin 0014C62A
    std::function<QVariant(const QModelIndex, int )> dataFunc;
    dataFunc = [](const QModelIndex & index, int role)
    {
            TreeItemInterface *item = static_cast<TreeItemInterface*>(index.internalPointer());
            if(!item)
                return QVariant();
            if ( role == Qt::CheckStateRole && index.column() == 0 )
                return  QVariant(static_cast< int >( item->checkState() ? Qt::Checked : Qt::Unchecked ));
            if(role == Qt::ForegroundRole)
                return QVariant(item->data(index.column(),role));
            if(role == Qt::ToolTipRole)
                return QVariant(item->data(index.column(),role));
            if(role == Qt::DisplayRole || role == Qt::EditRole)
                return QVariant(item->data(index.column(),role));
            if(role == Qt::SizeHintRole)
                return  QVariant(QSize(100, 20));
            return QVariant();
    };
    defaultRoleFunc = dataFunc;
    // Bouml preserved body end 0014C62A
}

TreeModel::~TreeModel()
{
    // Bouml preserved body begin 0014C6AA
    // Bouml preserved body end 0014C6AA
}

QVariant TreeModel::data(const QModelIndex & index, int role) const
{
    // Bouml preserved body begin 0014C72A
    if (!index.isValid())
        return QVariant();

    if(role == Qt::CheckStateRole && !(flags(index) & Qt::ItemIsUserCheckable))
        return QVariant();
    if(roleFuncs.contains(role))
        return roleFuncs[role](index);
    else if(defaultRoleFunc)
        return defaultRoleFunc(index, role);

    else
        return QVariant();
    // Bouml preserved body end 0014C72A
}

bool TreeModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    // Bouml preserved body begin 0014C7AA
    bool result = false;
    switch (role)
    {
        case Qt::CheckStateRole:
        {
            if (flags(index) & Qt::ItemIsUserCheckable)
            {
                TreeItemInterface *item = static_cast<TreeItemInterface*>(index.internalPointer());
                item->setCheckState(static_cast<Qt::CheckState>(value.toInt()));
                emit dataChanged(index, index, {role});
                emit itemCheckStatusChanged(index);
                result = true;
            }
            break;
        }
        default:
        {
            TreeItemInterface* item = dynamic_cast<TreeItemInterface*>(getItem(index));
            // TODO BUG dataChanged() emitted for single cell only, but setData() can have
            //   side effects, e.g. row background/foreground color can be changed
            if (item->setData(index.column(), value, role))
            {
                emit dataChanged(index, index, {role});
                if (role == Qt::EditRole)
                    emit dataEditFinished(index);
                result = true;
            }
            break;
        }
    }
    return result;
    // Bouml preserved body end 0014C7AA
}

Qt::ItemFlags TreeModel::flags(const QModelIndex & index) const
{
    // Bouml preserved body begin 0014C82A

    if (!index.isValid())
        return 0;

    //Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    Qt::ItemFlags flags = rootItem->flags(index);

//    if ( index.column() == 0 )
//        flags |= Qt::ItemIsUserCheckable;

    return flags;
    // Bouml preserved body end 0014C82A
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Bouml preserved body begin 0014C8AA
    if(rootItem == 0 )
        return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->GetColumns().at(section);
    return QVariant();
    // Bouml preserved body end 0014C8AA
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex & parent) const
{
    // Bouml preserved body begin 0014C92A

    //TRACE_FUNCTION;
    if(!rootItem)
        return QModelIndex();
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItemInterface *parentItem;

    if (!parent.isValid())
        parentItem = rootItem.get();
    else
        parentItem = static_cast<TreeItemInterface*>(parent.internalPointer());

    TreeItemInterface *childItem = static_cast<TreeItemInterface*>(parentItem->child(row));
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
    // Bouml preserved body end 0014C92A
}

QModelIndex TreeModel::parent(const QModelIndex & index) const
{
    // Bouml preserved body begin 0014C9AA

    //TRACE_FUNCTION;
    if (!index.isValid())
        return QModelIndex();

    TreeItemInterface *childItem = static_cast<TreeItemInterface*>(index.internalPointer());
    TreeItemInterface *parentItem = dynamic_cast<TreeItemInterface*>(childItem->parent());

    if (parentItem == rootItem.get())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
    // Bouml preserved body end 0014C9AA
}

int TreeModel::rowCount(const QModelIndex & parent) const
{
    // Bouml preserved body begin 0014CA2A
    //TRACE_FUNCTION;
    if(!rootItem)
        return 0;
    TreeItemInterface *parentItem;
    //    if (parent.column() > 0)
    //        return 0;

    if (!parent.isValid())
        parentItem = rootItem.get();
    else
        parentItem = static_cast<TreeItemInterface*>(parent.internalPointer());

    auto childCount = parentItem->childCount();
    //QLOG_INFO() << "Returning children count for " << modelName << ": " << childCount;

    return childCount;
    // Bouml preserved body end 0014CA2A
}

int TreeModel::columnCount(const QModelIndex & parent) const
{
    // Bouml preserved body begin 0014CAAA
    //TRACE_FUNCTION;
    if(!rootItem)
        return 0;
    if (parent.isValid())
        return static_cast<TreeItemInterface*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
    // Bouml preserved body end 0014CAAA
}

TreeItemInterface * TreeModel::RootItem()
{
    // Bouml preserved body begin 0014CB2A
    //TRACE_FUNCTION;
    return rootItem.get();
    // Bouml preserved body end 0014CB2A
}

void TreeModel::InsertRootItem(std::shared_ptr<TreeItemInterface> item)
{
    reset();
    if(rootItem && rootItem->childCount() > 0)
    {
        int prevRows = rootItem->childCount();
        int prevColumns = rootItem->columnCount();
        beginRemoveRows(QModelIndex(), 0, prevRows-1);
        endRemoveRows();
        emit dataChanged(index(0,0), index(prevRows-1, prevColumns));
    }

    rootItem = item;
    if(item && item->childCount() > 0)
    {
        int lastRowToInsert = item->childCount()-1;
        int columnCount = item->columnCount();
        beginInsertRows(QModelIndex(), 0, lastRowToInsert);
        endInsertRows();
        emit dataChanged(index(0,0), index(lastRowToInsert, columnCount));
    }
}


TreeItemInterface * TreeModel::getItem(const QModelIndex & index) const
{
    // Bouml preserved body begin 0014CC2A
    //TRACE_FUNCTION;
    if (index.isValid()) {
        TreeItemInterface *item = static_cast<TreeItemInterface*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem.get();
    // Bouml preserved body end 0014CC2A
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex & parent)
{
    // Bouml preserved body begin 0014CCAA
    //TRACE_FUNCTION;
    TreeItemInterface *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    if(!factoryFunc)
        success = parentItem->insertChildren(position, rows);
    else
        success = parentItem->insertChildren(position, rows, factoryFunc);
    endInsertRows();

    //emit dataChanged(this->index(position, 0, parent), this->index(position + rows - 1, columnCount(), parent));
    return success;
    // Bouml preserved body end 0014CCAA
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex & parent)
{
    // Bouml preserved body begin 0014CD2A
    //TRACE_FUNCTION;
    TreeItemInterface *parentItem = getItem(parent);
    beginRemoveRows(parent, position, position + rows - 1);
    bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    //emit dataChanged(this->index(position, 0, parent), this->index(position + rows - 1, columnCount(), parent));

    return success;
    // Bouml preserved body end 0014CD2A
}

void TreeModel::reset()
{
    QAbstractItemModel::beginResetModel();
    QAbstractItemModel::endResetModel();
}

void TreeModel::Refresh()
{
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

void TreeModel::AddRoleFunc(int role, std::function<QVariant (const QModelIndex &)> func)
{
    roleFuncs.insert(role, func);
}

void TreeModel::AddDefaultRoleFunc(std::function<QVariant (const QModelIndex & index, int role)> func)
{
    defaultRoleFunc = func;
}

void TreeModel::SetFactoryFunc(std::function<void(TreeItemInterface *,TreeItemInterface*)> func)
{
    factoryFunc = func;
}

void TreeModel::SetModelName(const QString &value)
{
    modelName = value;
}
