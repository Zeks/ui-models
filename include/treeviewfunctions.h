#ifndef TREEFUNCTIONS_H
#define TREEFUNCTIONS_H
#include <QList>
#include <QModelIndex>
#ifdef GUI_APPLICATION
#include <QAbstractItemView>
#endif
#include "l_tree_controller_global.h"


class QTreeView;
namespace TreeFunctions
{
enum class ENodeInsertionMode
{
    sibling,
    child
};


/*
 * Информация о том как именно вставляется потомок. Нодой или папкой
 **/


/* Рекурсивно восстанавливает состояние нод дерева
 * @paths список открытых нод, например ["root/row3", "root/row3/row0"]
 * @view отображение в котором требуется восстановить состояние
 **/
L_TREE_CONTROLLER_EXPORT void ApplyPathsOfExpandedNodes(
        QStringList paths, QTreeView* view);
/* Рекурсивно сохраняет состояние нод дерева
 * @paths список открытых нод, например ["root/row3", "root/row3/row0"]
 * @view отображение в котором требуется сохранить состояние
 **/
L_TREE_CONTROLLER_EXPORT void StorePathsOfExpandedNodes(
        QStringList& paths, QTreeView* view);
}
#endif // TREEFUNCTIONS_H
