#pragma once

#include "hash_tuple.h"

#include <QStringList>
#include <QVariant>
#include <QModelIndex>
#include <unordered_map>
#include <vector>

#include <type_traits>

#include "l_tree_controller_global.h"

#include <functional>



template<class T>
class ItemController
{
  public:
    using InnerType = typename std::remove_pointer_t<T>;
    using GetterFunctor = std::function<QVariant(const InnerType*)>;
    using SetterFunctor = std::function<bool(InnerType*, QVariant)>;
    inline QStringList GetColumns() const;

    void SetColumns(QStringList value);

    QVariant GetValue(const InnerType * item, int index, int role);

    bool SetValue(InnerType * item, int column, const QVariant & value, int role);

    void AddGetter(const std::pair<int,int> & index,  GetterFunctor function);
    void AddGetter(int row, const std::vector<int> &roles, GetterFunctor function);

    void AddWholeRowGetter(int role,  GetterFunctor function);
    void AddWholeRowGetter(const std::vector<int> &roles, GetterFunctor function);



    void AddSetter(const std::pair<int,int> & index, SetterFunctor function);
    void AddSetter(int row, const std::vector<int> &roles, SetterFunctor function);


    Qt::ItemFlags flags(const QModelIndex & index) const;

    static inline std::vector<std::function<Qt::ItemFlags(const QModelIndex&)>> GetFlagsFunctors();

    static void SetFlagsFunctors(std::vector<std::function<Qt::ItemFlags(const QModelIndex &)>> value);

    static void AddFlagsFunctor(std::function<Qt::ItemFlags(const QModelIndex&)> functor);

    static void SetDefaultTreeFlagFunctor();

  private:
     QStringList columns;
     std::unordered_map<std::pair<int, int>, GetterFunctor, hash_tuple::pair_hash> getters;
     std::unordered_map<int, GetterFunctor> wholeRowGetters;
     std::unordered_map<std::pair<int,int>, SetterFunctor, hash_tuple::pair_hash> setters;
     static std::vector<std::function<Qt::ItemFlags(const QModelIndex&)>> flagsFunctors;

};
template<class T>
std::vector<std::function<Qt::ItemFlags(const QModelIndex&)>> ItemController<T>::flagsFunctors;

template<class T>
inline QStringList ItemController<T>::GetColumns() const
{
    return columns;
}

template<class T>
void ItemController<T>::SetColumns(QStringList value)
{
    columns = value;
}

template<class T>
QVariant ItemController<T>::GetValue(const InnerType * item, int index, int role)
{
    const auto key = std::pair<int,int>(index, role);
    {
        auto it = std::find(getters.begin(),getters.end(),key);
        if(it!=getters.end())
            return it->second(item);
    }
    {
        auto it = std::find(wholeRowGetters.begin(),wholeRowGetters.end(),key);
        if(it!=wholeRowGetters.end())
            return it->second(item);
    }
    return {};
}

template<class T>
bool ItemController<T>:: SetValue(InnerType * item, int column, const QVariant & value, int role)
{
    const auto key = std::pair<int,int>(column, role);
    auto it = std::find(setters.begin(),setters.end(),key);
    if(it!=setters.end())
        return it->second(item, value);
    return false;
}

template<class T>
void ItemController<T>::AddGetter(const std::pair<int,int> & index, GetterFunctor function)
{
    getters[index] = function;
}

template<class T>
void ItemController<T>::AddGetter(int column, const std::vector<int> & roles, GetterFunctor function)
{
    for(int role : roles)
        AddGetter(std::pair<int,int>(column, role), function);
}

template<class T>
void ItemController<T>::AddWholeRowGetter(int role,  GetterFunctor function)
{
    wholeRowGetters.insert(role, function);
}

template<class T>
void ItemController<T>::AddWholeRowGetter(const std::vector<int>& roles, GetterFunctor function)
{
    for(int role : roles)
        AddWholeRowGetter(role, function);
}

template<class T>
void ItemController<T>::AddSetter(const std::pair<int,int> & index, SetterFunctor function)
{
    setters[index] = function;
}

template<class T>
void ItemController<T>::AddSetter(int row, const std::vector<int> & roles, SetterFunctor function)
{
    for(int role : roles)
        AddSetter(QPair<int,int>(row, role), function);
}

template<class T>
Qt::ItemFlags ItemController<T>::flags(const QModelIndex & index) const
{
    Qt::ItemFlags result;
    for(auto& func: flagsFunctors)
        result |= func(index);

    return result;
}

template<class T>
inline std::vector<std::function<Qt::ItemFlags(const QModelIndex&)>> ItemController<T>::GetFlagsFunctors()
{
    return flagsFunctors;
}

template<class T>
void ItemController<T>::SetFlagsFunctors(std::vector<std::function<Qt::ItemFlags(const QModelIndex&)>> value)

{
    flagsFunctors = value;
}

template<class T>
void ItemController<T>::AddFlagsFunctor(std::function<Qt::ItemFlags(const QModelIndex&)> functor)

{
    flagsFunctors.push_back(functor);
}

template<class T>
void ItemController<T>::SetDefaultTreeFlagFunctor()
{
    AddFlagsFunctor([](const QModelIndex& index)
    {
        Qt::ItemFlags flags;
        if ( index.column() == 0 )
            flags |= Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;
        return flags;
    }
    );
}

