#ifndef _ADAPTINGTABLEMODELPRIVATE_H
#define _ADAPTINGTABLEMODELPRIVATE_H


#include <QSharedPointer>

#include "l_tree_controller_global.h"

#include "AdaptingTableModel.h"
class AdaptingTableModel;
class TableDataInterface;


class   AdaptingTableModelPrivate  
{
friend class AdaptingTableModel;
  public:
    explicit AdaptingTableModelPrivate(AdaptingTableModel* q);

    virtual ~AdaptingTableModelPrivate();


  private:
     AdaptingTableModel * q = nullptr;
    QSharedPointer<TableDataInterface> interface;

};
#endif
