#include "TreeFilteringCache.h"

void TreeFilteringCache::Reset(std::shared_ptr<TreeItemInterface> rootItem)
{
    this->rootItem = rootItem;
    cached.clear();
}

void TreeFilteringCache::Cache(QString filter, std::shared_ptr<TreeItemInterface> rootItem)
{
    cached[filter] = rootItem;
}

std::shared_ptr<TreeItemInterface> TreeFilteringCache::GetMostMatched(QString filter, bool *exactlyMatched) const
{
    if (exactlyMatched)
        *exactlyMatched = filter.isEmpty() || cached.contains(filter);

    if (filter.isEmpty())
        return rootItem;
    else if (cached.contains(filter))
        return cached[filter];

    if (filter.length() > 1)
    {
        QString subfilter = filter.left(filter.length() - 1);
        if (cached.contains(subfilter))
            return cached[subfilter];
    }
    return rootItem;
}
