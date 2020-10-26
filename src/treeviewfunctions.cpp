#include "include/treeviewfunctions.h"
#ifdef GUI_APPLICATION
#include <QTreeView>

#include <functional>

#include "logger/QsLogger.h"

namespace TreeFunctions {

inline QString childIndexPath(QString parentPath, const QModelIndex& childIndex)
{
    QString result = parentPath + "/row" + QString::number(childIndex.row());
    return result;
}

void ApplyPathsOfExpandedNodesImpl(
        QStringList nodes, QTreeView* view, QAbstractItemModel* model,
        QModelIndex parentIndex, QString parentPath)
{
    for (int i = 0; i < model->rowCount(parentIndex); ++i)
    {
        QModelIndex childIndex = model->index(i, 0, parentIndex);
        QString childPath = childIndexPath(parentPath, childIndex);
        if (nodes.contains(childPath))
            view->setExpanded(childIndex, true);
        ApplyPathsOfExpandedNodesImpl(nodes, view, model, childIndex, childPath);
    }
}

void ApplyPathsOfExpandedNodes(QStringList nodes, QTreeView* view)
{
    ApplyPathsOfExpandedNodesImpl(nodes, view, view->model(), QModelIndex(), "root");
}

void StorePathsOfExpandedNodesImpl(
        QStringList& nodes, QTreeView* view, QAbstractItemModel* model,
        QModelIndex parentIndex, QString parentPath)
{
    for(int i = 0; i < model->rowCount(parentIndex); ++i)
    {
        QModelIndex childIndex = model->index(i, 0, parentIndex);
        QString childPath = childIndexPath(parentPath, childIndex);
        if (view->isExpanded(childIndex))
            nodes << childPath;
        StorePathsOfExpandedNodesImpl(nodes, view, model, childIndex, childPath);
    }
}

void StorePathsOfExpandedNodes(QStringList& paths, QTreeView* view)
{
    StorePathsOfExpandedNodesImpl(paths, view, view->model(), QModelIndex(), "root");
}

} // namespace TreeFunctions

#endif
