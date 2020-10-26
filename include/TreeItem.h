#pragma once


#include <QList>
#include <QSharedPointer>
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
typedef TreeItem<T> value_type;
typedef T template_value_type;
  public:
    TreeItem(const T & data);

    //TreeItem(std::function<void(TreeItemInterface*)>factoryFunc);

    explicit TreeItem();

    virtual QList<std::shared_ptr<TreeItemInterface>> GetChildren();
    virtual QList<std::shared_ptr<TreeItemInterface>>& GetChildrenRef();

    void ResetState(std::true_type);
    void ResetState(std::false_type);

    void AssignNewData(typename std::remove_pointer<T>::type, std::true_type);
    void AssignNewData(typename std::remove_pointer<T>::type, std::false_type);
    void AssignNewData(typename std::remove_pointer<T>::type*, std::true_type);
    void AssignNewData(typename std::remove_pointer<T>::type*, std::false_type);

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


    virtual QList<std::shared_ptr<TreeItemInterface>>& children();

    virtual const QList<std::shared_ptr<TreeItemInterface>>& children() const;

    void addChildren(QList<std::shared_ptr<TreeItemInterface>> _children);

    void addChild(std::shared_ptr<TreeItemInterface> _child);

    inline QSharedPointer<ItemController<typename std::remove_pointer<T>::type *>> GetController() const;

    void SetController(QSharedPointer<ItemController<typename std::remove_pointer<T>::type *>> value);

    virtual QStringList GetColumns();

    virtual int IndexOf(TreeItemInterface * item);

    void SetInternalData(typename std::remove_pointer<T>::type * _data);
    void SetInternalData(const typename std::remove_pointer<T>::type  & _data);

    void* InternalPointer();

    virtual TreeItemInterface* child(int row);

    virtual TreeItemInterface* parent();

    virtual int row();

    template <class X = T>
    typename std::enable_if<std::is_pointer<T>::value, X>::type
    GetPointer()
    {
       return m_data;
    }

    template <class X = T>
    typename std::enable_if<!std::is_pointer<T>::value, X>::type*
    GetPointer()
    {
       return &m_data;
    }


    virtual void SetParent(std::shared_ptr<TreeItemInterface> _parent);

    int GetIndexOfChild(TreeItemInterface * child);

    virtual Qt::ItemFlags flags(const QModelIndex & index) const;

    virtual void AddChildren(QList<std::shared_ptr<TreeItemInterface>> children, int insertAfter = 0);
    bool AreChildrenExclusive()  const {return childrenExclusive;}
    void SetChildrenExclusive(bool value) {childrenExclusive = value;}

  private:
     T m_data;

  protected:
     QList<std::shared_ptr<TreeItemInterface>> m_children;
     std::weak_ptr<TreeItemInterface > m_parent;

private:
     bool m_isCheckable;
     Qt::CheckState m_checkState;
     QSharedPointer<ItemController<typename std::remove_pointer<T>::type * >> controller;
     QFont font;
     bool childrenExclusive = false;
};
template<class T>
TreeItem<T>::TreeItem(const T & data) : TreeItemInterface()
{
    // Bouml preserved body begin 0020A62A
    ResetState(std::is_pointer<T>());
    m_data = data;
    // Bouml preserved body end 0020A62A
}

template<class T>
TreeItem<T>::TreeItem() : TreeItemInterface()
{
    // Bouml preserved body begin 0020A72A
    ResetState(std::is_pointer<T>());
    // Bouml preserved body end 0020A72A
}

template<class T>
QList<std::shared_ptr<TreeItemInterface>> TreeItem<T>::GetChildren()
{
    // Bouml preserved body begin 002202AA
    return m_children;
    // Bouml preserved body end 002202AA
}

template<class T>
QList<std::shared_ptr<TreeItemInterface>>& TreeItem<T>::GetChildrenRef()
{
    // Bouml preserved body begin 002202AA
    return m_children;
    // Bouml preserved body end 002202AA
}

template<class T>
void TreeItem<T>::ResetState(std::true_type)
{
    // Bouml preserved body begin 0021CE2A
    m_checkState = Qt::Unchecked;
    m_data = nullptr;
    m_children.clear();
    m_parent.reset();
    m_isCheckable = true;
    controller = QSharedPointer<ItemController<typename std::remove_pointer<T>::type * >>();
    // Bouml preserved body end 0021CE2A
}
template<class T>
void TreeItem<T>::ResetState(std::false_type)
{
    // Bouml preserved body begin 0021CE2A
    m_checkState = Qt::Unchecked;
    m_data = T();
    m_children.clear();
    m_parent.reset();
    m_isCheckable = true;
    controller = QSharedPointer<ItemController<typename std::remove_pointer<T>::type * >>();
    // Bouml preserved body end 0021CE2A
}

template<class T>
void TreeItem<T>::AssignNewData(typename std::remove_pointer<T>::type , std::true_type){
    Q_ASSERT(false);
}
template<class T>
void TreeItem<T>::AssignNewData(typename std::remove_pointer<T>::type* data, std::true_type){
    m_data = data;
}

template<class T>
void TreeItem<T>::AssignNewData(typename std::remove_pointer<T>::type data, std::false_type){
    m_data = data;
}
template<class T>
void TreeItem<T>::AssignNewData(typename std::remove_pointer<T>::type* data, std::false_type){
    m_data = *data;
}
template<class T>
void TreeItem<T>::SetInternalData(const typename std::remove_pointer<T>::type  & _data)
{
    // Bouml preserved body begin 0020A7AA
    AssignNewData(_data, std::is_pointer<T>());
    // Bouml preserved body end 0020A7AA
}
template<class T>
void TreeItem<T>::SetInternalData(typename std::remove_pointer<T>::type * _data)
{
    // Bouml preserved body begin 0020A7AA
    AssignNewData(_data, std::is_pointer<T>());
    //m_data = _data;
    // Bouml preserved body end 0020A7AA
}

template<class T>
TreeItem<T>::~TreeItem()
{
    // Bouml preserved body begin 0020A6AA
    // Bouml preserved body end 0020A6AA
}

template<class T>
int TreeItem<T>::columnCount() const
{
    // Bouml preserved body begin 0020402A
    return controller->GetColumns().size();
    // Bouml preserved body end 0020402A
}

template<class T>
int TreeItem<T>::childCount() const
{
    // Bouml preserved body begin 002040AA
    return m_children.size();
    // Bouml preserved body end 002040AA
}

template<class T>
QVariant TreeItem<T>::data(int column, int role)
{
    // Bouml preserved body begin 00203FAA
    if(role == Qt::FontRole)
        return font;
    return controller->GetValue(GetPointer<T>(), column, role);
    // Bouml preserved body end 00203FAA
}

template<class T>
bool TreeItem<T>::isCheckable() const
{
    // Bouml preserved body begin 00203F2A
    return m_isCheckable;
    // Bouml preserved body end 00203F2A
}

template<class T>
void TreeItem<T>::setCheckState(Qt::CheckState state)
{
    // Bouml preserved body begin 00203EAA
    m_checkState = state;
    // Bouml preserved body end 00203EAA
}

template<class T>
Qt::CheckState TreeItem<T>::checkState()
{
    // Bouml preserved body begin 00203E2A
    return m_checkState;
    // Bouml preserved body end 00203E2A
}

template<class T>
bool TreeItem<T>::setData(int column, const QVariant & value, int role)
{
    // Bouml preserved body begin 00203D2A
    if(role == Qt::FontRole)
    {
        font = qvariant_cast<QFont>(value);
        return true;
    }
    bool result = controller->SetValue(GetPointer<T>(), column,value, role);
    return result;
    // Bouml preserved body end 00203D2A
}

template<class T>
bool TreeItem<T>::removeChildren(int position, int count)
{
    // Bouml preserved body begin 00203CAA
    if (position < 0 || position > m_children.size())
        return false;
    //qDebug() << Q_FUNC_INFO;
    auto leftBound = m_children.begin()+position;
    auto rightBound = leftBound + (count);
    m_children.erase(leftBound,rightBound);
    return true;
    // Bouml preserved body end 00203CAA
}

template<class T>
bool TreeItem<T>::insertChildren(int position, int count)
{
    // Bouml preserved body begin 00203C2A
    if (position < 0 || position > m_children.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        std::shared_ptr<TreeItemInterface> item (new TreeItem<T>());
        m_children.insert(position, item);
        //item->SetParent(this);
    }

    return true;
    // Bouml preserved body end 00203C2A
}

template<class T>
bool TreeItem<T>::insertChildren(int position, int count, std::function<void (TreeItemInterface *,TreeItemInterface *)> factory)
{
    if (position < 0 || position > m_children.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        auto internal = new TreeItem<T>();
        std::shared_ptr<TreeItemInterface> item (internal);
        factory(this, item.get());
        internal->SetController(controller);
        item->SetParent(this->shared_from_this());
        m_children.insert(position, item);
        //item->SetParent(this);
    }

    return true;
}

template<class T>
QList<std::shared_ptr<TreeItemInterface>>& TreeItem<T>::children()
{
    // Bouml preserved body begin 0020432A
    return m_children;
    // Bouml preserved body end 0020432A
}

template<class T>
const QList<std::shared_ptr<TreeItemInterface>>& TreeItem<T>::children() const
{
    // Bouml preserved body begin 002042AA
    return m_children;
    // Bouml preserved body end 002042AA
}

template<class T>
void TreeItem<T>::addChildren(QList<std::shared_ptr<TreeItemInterface>> _children)
{
    // Bouml preserved body begin 0020422A
    m_children.append(_children);
//    for(auto child: _children)
//    {
//        if(!m_parent.isNull())
//            child->SetParent(m_parent.toStrongRef().child(m_parent.toStrongRef()->GetIndexOfChild(this)));
//    }
    // Bouml preserved body end 0020422A
}
template<class T>
void TreeItem<T>::addChild(std::shared_ptr<TreeItemInterface> _child)
{
    m_children.append(_child);
}

template<class T>
inline QSharedPointer<ItemController<typename std::remove_pointer<T>::type *>> TreeItem<T>::GetController() const
{
    return controller;
}

template<class T>
void TreeItem<T>::SetController(QSharedPointer<ItemController<typename std::remove_pointer<T>::type * >> value)
{
    controller = value;
}

template<class T>
QStringList TreeItem<T>::GetColumns()
{
    // Bouml preserved body begin 00206CAA
    return controller->GetColumns();
    // Bouml preserved body end 00206CAA
}

template<class T>
int TreeItem<T>::IndexOf(TreeItemInterface * item)
{
    // Bouml preserved body begin 00206D2A
    return GetIndexOfChild(item);
    // Bouml preserved body end 00206D2A
}


template<class T>
void* TreeItem<T>::InternalPointer()
{
    // Bouml preserved body begin 002086AA
    return static_cast<void*>(GetPointer<T>());
    // Bouml preserved body end 002086AA
}

template<class T>
TreeItemInterface* TreeItem<T>::child(int row)
{
    // Bouml preserved body begin 0020CD2A
    return m_children.at(row).get();
    // Bouml preserved body end 0020CD2A
}

template<class T>
TreeItemInterface* TreeItem<T>::parent()
{
    // Bouml preserved body begin 0020CDAA
    return m_parent.lock().get();
    // Bouml preserved body end 0020CDAA
}

template<class T>
int TreeItem<T>::row()
{
    // Bouml preserved body begin 0020CEAA
    if (!m_parent.expired())
        return m_parent.lock().get()->IndexOf(this);
    return 0;
    // Bouml preserved body end 0020CEAA
}

template<class T>
void TreeItem<T>::SetParent(std::shared_ptr<TreeItemInterface> _parent)
{
    // Bouml preserved body begin 0020CFAA
    m_parent = _parent;
    // Bouml preserved body end 0020CFAA
}

template<class T>
int TreeItem<T>::GetIndexOfChild(TreeItemInterface * child)
{
    // Bouml preserved body begin 0020D02A
    for(int i(0); i < m_children.size(); ++i)
    {
        if(m_children.at(i).get() == child)
            return i;
    }
    return -1;
    // Bouml preserved body end 0020D02A
}

template<class T>
Qt::ItemFlags TreeItem<T>::flags(const QModelIndex & index) const
{
    // Bouml preserved body begin 0021B2AA
    return controller->flags(index);
    // Bouml preserved body end 0021B2AA
}

template<class T>
void TreeItem<T>::AddChildren(QList<std::shared_ptr<TreeItemInterface>> children, int insertAfter)
{
    // Bouml preserved body begin 002201AA
    //m_children.insert(m_children.begin() + insertAfter, children);
    //m_children.reserve(m_children.size() + children.size());
    int i(0);
    for(auto child : children)
    {
        m_children.insert(m_children.begin() + insertAfter + i, child);
        i++;
    }
    // Bouml preserved body end 002201AA
}



