#pragma once

#include <QSharedPointer>

#include "l_tree_controller_global.h"
#include <memory>

class TreeItemInterface;

class L_TREE_CONTROLLER_EXPORT TreeFilteringCache
{
public:
    void Reset(std::shared_ptr<TreeItemInterface> rootItem);
    void Cache(QString filter, std::shared_ptr<TreeItemInterface> rootItem);
    std::shared_ptr<TreeItemInterface> GetMostMatched(QString filter, bool* exactlyMatched = nullptr) const;

private:
    std::shared_ptr<TreeItemInterface> rootItem;
    QHash<QString, std::shared_ptr<TreeItemInterface>> cached;
};
