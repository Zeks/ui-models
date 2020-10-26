// *************************************************************************
//
// Copyright 2012-2013 Nikolai Marchenko.
//
// This file is part of the Douml Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License Version 3.0 as published by
// the Free Software Foundation and appearing in the file LICENSE.GPL included in the
//  packaging of this file.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License Version 3.0 for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : doumleditor@gmail.com
//
// *************************************************************************
#ifndef TREEITEMFUNCTIONS_H
#define TREEITEMFUNCTIONS_H
#include "UniversalModels/include/TreeItemInterface.h"
#include "logger/QsLog.h"
#include <assert.h>
namespace TreeFunctions
{
template<typename K>
using RootChecks = QList<std::function<bool(K*)>>;
template<typename K>
using ModifierFunction = std::function<void(K*)>;
template<typename K>
using ModifierFunctions = QList<ModifierFunction<K>>;


/* Отфильтровывает из дерева ноды которые удовлетворяют условиям проверок
 * @rootItem верхний уровень дерева от которого производится фильтрация
 * @filterFunctions список проверок которым должна удовлетворять нода
 * @forcePick говорит о том, что ноду нужно зацепить в любом случае(когда нода-папка удовлетворяет условию)
 **/
template<typename InterfaceType, typename ConcreteItemType>
std::shared_ptr<InterfaceType> RecursiveGetSubset(InterfaceType* rootItem,
                                                 RootChecks<InterfaceType> filterFunctions,
                                                 bool forcePick = false, bool pickDescendantsOfValidNodes = false)
{
    QList<std::shared_ptr<InterfaceType>> newChildren;
    bool nodeValid = true;
    if(!(forcePick && pickDescendantsOfValidNodes))
    {
        for(std::function<bool(InterfaceType*)> filter: filterFunctions)
        {
            if(!filter(rootItem))
            {
                nodeValid = false;
                break;
            }
            else
                nodeValid = true;
        }
    }
    if(!rootItem->GetChildren().isEmpty())
    {
        for(std::shared_ptr<InterfaceType> child: rootItem->GetChildren())
        {
            std::shared_ptr<InterfaceType> newChild = RecursiveGetSubset<InterfaceType,ConcreteItemType>(child.get(), filterFunctions, nodeValid, pickDescendantsOfValidNodes);
            if(newChild)
                newChildren += newChild;
        }
    }
    std::shared_ptr<InterfaceType> newItem;
    bool hasValidDescendants = !newChildren.isEmpty();
    if(nodeValid || hasValidDescendants || (forcePick && pickDescendantsOfValidNodes))
    {
        //newItem = std::shared_ptr<InterfaceType>(new ConcreteItemType(*dynamic_cast<ConcreteItemType*>(rootItem)));

        std::unique_ptr<ConcreteItemType> tmp(new ConcreteItemType);
        auto concreteRoot = dynamic_cast<ConcreteItemType*>(rootItem);
        tmp->SetInternalData(concreteRoot->GetPointer());
        tmp->SetController(concreteRoot->GetController());
        newItem.reset(tmp.release());



        for(std::shared_ptr<InterfaceType> child: newChildren)
        {
            child->SetParent(newItem);
        }
        //QLOG_DEBUG() << "Adding children:" << newChildren.size();
        newItem->AddChildren(newChildren);
    }
    return newItem;
}


template<typename InterfaceType>
QList<InterfaceType*> FlattenHierarchy(InterfaceType* rootItem, RootChecks<InterfaceType> filterFunctions, bool forcePick = false)
{
    QList<InterfaceType* > result;

    QList<std::shared_ptr<InterfaceType>> newChildren;
    bool nodeValid = true;
    for(std::function<bool(InterfaceType*)> filter: filterFunctions)
    {
       if(filter(rootItem))
       {
           nodeValid = false;
           break;
       }
    }
    if(!rootItem->GetChildren().isEmpty())
    {
        for(std::shared_ptr<InterfaceType> child: rootItem->GetChildren())
        {
            if(child)
                result.append(FlattenHierarchy<InterfaceType>(child.get(), filterFunctions, nodeValid));
        }
    }
    if(rootItem != 0 && (nodeValid || forcePick))
    {
        result+=rootItem;
    }
    return result;
}


template<typename InterfaceType>
bool RecursiveSearch(InterfaceType* rootItem, RootChecks<InterfaceType> filterFunctions)
{
    bool result = true;
    for(std::function<bool(InterfaceType*)> filter: filterFunctions)
    {
       if(!filter(rootItem))
       {
           result = false;
           break;
       }
    }
    if(result)
        return true;
    if(!rootItem->GetChildren().isEmpty())
    {
        for(std::shared_ptr<InterfaceType> child: rootItem->GetChildren())
        {
            bool result = RecursiveSearch<InterfaceType>(child.data(), filterFunctions);
            if(result)
                return true;
        }
    }
    return false;
}

template<typename InterfaceType>
void ModifyNode(InterfaceType* node, ModifierFunctions<InterfaceType> modifierFunctions)
{
    if(!node)
        return;
    for(ModifierFunction<InterfaceType> func : modifierFunctions)
    {
        func(node);
    }

}

template<typename InterfaceType>
bool RecursiveModify(InterfaceType* rootItem, RootChecks<InterfaceType> filterFunctions, ModifierFunctions<InterfaceType> modifierFunctions,
                     bool parentsFirst = true, bool stopAtFirstMatch = true )
{
    bool result = false;
    bool nodeValid = true;
    if(!rootItem)
        return false;
    for(std::function<bool(InterfaceType*)> filter: filterFunctions)
    {
       if(!filter(rootItem))
       {
           nodeValid = false;
           break;
       }
    }
    if(nodeValid && parentsFirst)
    {
        result = true;
        ModifyNode<InterfaceType>(rootItem,modifierFunctions);
        if(stopAtFirstMatch)
            return result;
    }

    if(!rootItem->GetChildren().isEmpty())
    {
        for(std::shared_ptr<InterfaceType> child: rootItem->GetChildren())
        {
            result = RecursiveModify<InterfaceType>(child.get(), filterFunctions, modifierFunctions, parentsFirst, stopAtFirstMatch);
            if(stopAtFirstMatch)
                return result;
        }
    }
    if(nodeValid && !parentsFirst)
    {
        result = true;
        ModifyNode<InterfaceType>(rootItem,modifierFunctions);
        if(stopAtFirstMatch)
            return result;
    }
    return result;
}




/* Отфильтровывает из дерева список свойств по переданному аксессору
 * @rootItem верхний уровень дерева от которого производится фильтрация
 * @accessor функция для доступа к конкретному члену структуры который надо выцепить
 **/
template<typename K, typename T>
QList<K> RecursiveGet(T* rootItem, std::function<K(T*)> accessor)
{
    QList<K> result;
    if(!rootItem)
        return result;
    if(!rootItem->GetChildren().isEmpty())
    {
        for(std::shared_ptr<T> child: rootItem->GetChildren())
        {
            result += RecursiveGet(child.get(), accessor);
        }
    }
    result.append(accessor(rootItem));
    return result;

}
/* Отфильтровывает из дерева ноды которые удовлетворяют условиям проверок
 * и помещает их в интерфейс для последующего запихивания в дерево
 *
 * @rootItem верхний уровень дерева от которого производится фильтрация
 * @filterFunctions список проверок которым должна удовлетворять нода
 **/
template<typename InterfaceType, template <typename> class ItemType, typename DataType>
std::shared_ptr<InterfaceType> FilterSubset(std::shared_ptr<InterfaceType> rootItem,
                                            QList<std::function<bool(InterfaceType*)>> filterFunctions,
                                            bool pickDescendants = false)
{
    typedef ItemType<DataType> ConcreteItemType;
    assert(rootItem);
    std::shared_ptr<InterfaceType> subsetRoot = TreeFunctions::RecursiveGetSubset<InterfaceType, ConcreteItemType>(
                rootItem.get(),
                filterFunctions,
                false,
                pickDescendants);
    if (subsetRoot)
        return subsetRoot;
    else
    {
        std::unique_ptr<ConcreteItemType> tmp(new ConcreteItemType);
        auto concreteRoot = dynamic_cast<ConcreteItemType*>(rootItem.get());
        tmp->SetInternalData(concreteRoot->GetPointer());
        tmp->SetController(concreteRoot->GetController());

        std::shared_ptr<InterfaceType> newRoot(tmp.release());
        //newRoot->removeChildren(0, newRoot->childCount());
        return newRoot;
    }
}

}
#endif // TREEITEMFUNCTIONS_H
