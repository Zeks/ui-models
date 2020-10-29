#pragma once

#include <QList>
#include <QSharedPointer>
#include <QVariant>
#include <QStringList>
#include <QModelIndex>

#include "l_tree_controller_global.h"

#include <QWeakPointer>
#include <QList>
#include <functional>
#include <memory>


class L_TREE_CONTROLLER_EXPORT TreeItemInterface : public std::enable_shared_from_this<TreeItemInterface>
{
  public:
    explicit TreeItemInterface();

    virtual ~TreeItemInterface();

    virtual QList<std::shared_ptr<TreeItemInterface>> GetChildren() = 0;
    virtual QList<std::shared_ptr<TreeItemInterface>>& GetChildrenRef() = 0;

    virtual int childCount() const = 0;

    virtual int columnCount() const = 0;

    virtual QVariant data(int column, int role) = 0;

    virtual bool isCheckable() const = 0;

    virtual void setCheckState(Qt::CheckState state) = 0;

    virtual Qt::CheckState checkState() = 0;

    virtual bool setData(int column, const QVariant & value, int role) = 0;

    virtual bool removeChildren(int position, int count) = 0;
    virtual bool removeChildren() = 0;

    virtual bool insertChildren(int position, int count) = 0;
    virtual bool insertChildren(int position, int count, std::function<void(TreeItemInterface*,TreeItemInterface*)>) = 0;

    virtual QStringList GetColumns() = 0;

    virtual void* InternalPointer() = 0;

    virtual TreeItemInterface* child(int row) = 0;

    virtual TreeItemInterface* parent() = 0;

    virtual int row() = 0;

    virtual void SetParent(std::shared_ptr<TreeItemInterface> _parent) = 0;

    virtual Qt::ItemFlags flags(const QModelIndex & index) const = 0;

    virtual void AddChildren(QList<std::shared_ptr<TreeItemInterface>> children, int insertAfter = 0) = 0;
    virtual void addChild(std::shared_ptr<TreeItemInterface> child) = 0;

    virtual int IndexOf(TreeItemInterface * item) = 0;

    virtual int GetIndexOfChild(TreeItemInterface * child) = 0;

    virtual bool AreChildrenExclusive() const = 0;

    virtual void SetChildrenExclusive(bool value) = 0;

    std::shared_ptr<TreeItemInterface> GetPtr() { return shared_from_this();}
};
