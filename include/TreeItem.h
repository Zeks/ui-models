#pragma once


#include <QList>
#include "TreeItemInterface.h"
#include <QVariant>
#include "ItemController.h"
#include <QStringList>
#include <QModelIndex>
#include <type_traits>

#include "l_tree_controller_global.h"

#include <QAbstractItemView>

template<class T>
class TreeItem : public TreeItemInterface
{
    using InnerType = T;
    using ItemPointerType = std::shared_ptr<TreeItemInterface>;
public:
    TreeItem(const T & data);

    //TreeItem(std::function<void(TreeItemInterface*)>factoryFunc);

    explicit TreeItem();

    virtual QList<ItemPointerType> GetChildren();
    virtual QList<ItemPointerType>& GetChildrenRef();

    void ResetState();

    template <class OwnType = InnerType, class AssignedType>
    void AssignNewData(AssignedType newData){
        if constexpr(std::is_pointer<AssignedType>::value && std::is_pointer<OwnType>::value)
            *m_data = *newData;
        else if constexpr(std::is_pointer<AssignedType>::value && !std::is_pointer<OwnType>::value)
            *m_data = newData;
        else if constexpr(!std::is_pointer<AssignedType>::value && std::is_pointer<OwnType>::value)
            m_data = *newData;
        else if constexpr(!std::is_pointer<AssignedType>::value && !std::is_pointer<OwnType>::value)
            m_data = newData;
    }

    virtual ~TreeItem();

    virtual int columnCount() const;

    virtual int childCount() const;

    virtual QVariant data(int column, int role);

    virtual bool isCheckable() const;

    virtual void setCheckState(Qt::CheckState state);

    virtual Qt::CheckState checkState();

    virtual bool setData(int column, const QVariant & value, int role);

    virtual bool removeChildren(int position, int count);

    virtual bool insertChildren(int position, int count);

    virtual bool insertChildren(int position, int count, std::function<void(TreeItemInterface*,TreeItemInterface*)>);


    virtual QList<ItemPointerType>& children();

    virtual const QList<ItemPointerType>& children() const;

    void addChildren(QList<ItemPointerType> _children);

    void addChild(ItemPointerType _child);

    inline std::shared_ptr<ItemController<InnerType> > GetController() const;

    void SetController(std::shared_ptr<ItemController<InnerType> > value);

    virtual QStringList GetColumns();

    virtual int IndexOf(TreeItemInterface * item);

    void SetInternalData(InnerType * _data);
    void SetInternalData(const InnerType  & _data);

    void* InternalPointer();

    virtual TreeItemInterface* child(int row);

    virtual TreeItemInterface* parent();

    virtual int row();

    InnerType* GetPointer(){
        if constexpr(std::is_pointer<InnerType>::value)
            return m_data;
        else
            return &m_data;
    }


    virtual void SetParent(ItemPointerType _parent);

    int GetIndexOfChild(TreeItemInterface * child);

    virtual Qt::ItemFlags flags(const QModelIndex & index) const;

    virtual void AddChildren(QList<ItemPointerType> children, int insertAfter = 0);
    bool AreChildrenExclusive()  const {return childrenExclusive;}
    void SetChildrenExclusive(bool value) {childrenExclusive = value;}

private:
    T m_data;

protected:
    QList<ItemPointerType> m_children;
    std::weak_ptr<TreeItemInterface > m_parent;

private:
    bool m_isCheckable;
    Qt::CheckState m_checkState;
    std::shared_ptr<ItemController<InnerType>> controller;
    QFont font;
    bool childrenExclusive = false;
};
template<class T>
TreeItem<T>::TreeItem(const T & data) : TreeItemInterface()
{
    ResetState();
    m_data = data;
}

template<class T>
TreeItem<T>::TreeItem() : TreeItemInterface()
{
    ResetState();
}

template<class T>
QList<typename TreeItem<T>::ItemPointerType> TreeItem<T>::GetChildren()
{
    return m_children;
}

template<class T>
QList<typename TreeItem<T>::ItemPointerType>& TreeItem<T>::GetChildrenRef()
{
    return m_children;
}

template<class T>
void TreeItem<T>::ResetState()
{
    m_checkState = Qt::Unchecked;
    if constexpr(std::is_pointer<T>::value)
        m_data = nullptr;
    else
        m_data = T();
    m_children.clear();
    m_parent.reset();
    m_isCheckable = true;
    controller = std::shared_ptr<ItemController<InnerType>>();
}

template<class T>
void TreeItem<T>::SetInternalData(const InnerType  & _data)
{
    AssignNewData(_data);
}
template<class T>
void TreeItem<T>::SetInternalData(InnerType * _data)
{
    AssignNewData(_data);
}

template<class T>
TreeItem<T>::~TreeItem()
{
}

template<class T>
int TreeItem<T>::columnCount() const
{
    return controller->GetColumns().size();
}

template<class T>
int TreeItem<T>::childCount() const
{
    return m_children.size();
}

template<class T>
QVariant TreeItem<T>::data(int column, int role)
{
    InnerType* pointer = GetPointer();
    return controller->GetValue(pointer, column, role);
}

template<class T>
bool TreeItem<T>::isCheckable() const
{
    return m_isCheckable;
}

template<class T>
void TreeItem<T>::setCheckState(Qt::CheckState state)
{
    m_checkState = state;
}

template<class T>
Qt::CheckState TreeItem<T>::checkState()
{
    return m_checkState;
}

template<class T>
bool TreeItem<T>::setData(int column, const QVariant & value, int role)
{
    auto pointer = GetPointer();
    bool result = controller->SetValue(pointer, column,value, role);
    return result;
}

template<class T>
bool TreeItem<T>::removeChildren(int position, int count)
{
    if (position < 0 || position > m_children.size())
        return false;
    //qDebug() << Q_FUNC_INFO;
    auto leftBound = m_children.begin()+position;
    auto rightBound = leftBound + (count);
    m_children.erase(leftBound,rightBound);
    return true;
}

template<class T>
bool TreeItem<T>::insertChildren(int position, int count)
{
    if (position < 0 || position > m_children.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        ItemPointerType item (new TreeItem<T>());
        m_children.insert(position, item);
        //item->SetParent(this);
    }

    return true;
}

template<class T>
bool TreeItem<T>::insertChildren(int position, int count, std::function<void (TreeItemInterface *,TreeItemInterface *)> factory)
{
    if (position < 0 || position > m_children.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        auto internal = new TreeItem<T>();
        ItemPointerType item (internal);
        factory(this, item.get());
        internal->SetController(controller);
        item->SetParent(this->shared_from_this());
        m_children.insert(position, item);
        //item->SetParent(this);
    }

    return true;
}

template<class T>
QList<typename TreeItem<T>::ItemPointerType>& TreeItem<T>::children()
{
    return m_children;
}

template<class T>
const QList<typename TreeItem<T>::ItemPointerType>& TreeItem<T>::children() const
{
    return m_children;
}

template<class T>
void TreeItem<T>::addChildren(QList<ItemPointerType> _children)
{
    m_children.append(_children);
}
template<class T>
void TreeItem<T>::addChild(ItemPointerType _child)
{
    m_children.append(_child);
}

template<class T>
inline std::shared_ptr<ItemController<typename TreeItem<T>::InnerType>> TreeItem<T>::GetController() const
{
    return controller;
}

template<class T>
void TreeItem<T>::SetController(std::shared_ptr<ItemController<InnerType>> value)
{
    controller = value;
}

template<class T>
QStringList TreeItem<T>::GetColumns()
{
    return controller->GetColumns();
}

template<class T>
int TreeItem<T>::IndexOf(TreeItemInterface * item)
{
    return GetIndexOfChild(item);
}


template<class T>
void* TreeItem<T>::InternalPointer()
{
    return static_cast<void*>(GetPointer());
}

template<class T>
TreeItemInterface* TreeItem<T>::child(int row)
{
    return m_children.at(row).get();
}

template<class T>
TreeItemInterface* TreeItem<T>::parent()
{
    return m_parent.lock().get();
}

template<class T>
int TreeItem<T>::row()
{
    if (!m_parent.expired())
        return m_parent.lock().get()->IndexOf(this);
    return 0;
}

template<class T>
void TreeItem<T>::SetParent(ItemPointerType _parent)
{
    m_parent = _parent;
}

template<class T>
int TreeItem<T>::GetIndexOfChild(TreeItemInterface * child)
{
    for(int i(0); i < m_children.size(); ++i)
    {
        if(m_children.at(i).get() == child)
            return i;
    }
    return -1;
}

template<class T>
Qt::ItemFlags TreeItem<T>::flags(const QModelIndex & index) const
{
    return controller->flags(index);
}

template<class T>
void TreeItem<T>::AddChildren(QList<ItemPointerType> children, int insertAfter)
{
    int i(0);
    for(auto child : children)
    {
        m_children.insert(m_children.begin() + insertAfter + i, child);
        i++;
    }
}



