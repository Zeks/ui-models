#pragma once
#include <QStyledItemDelegate>
#include <functional>
class CustomIconDelegate : public QStyledItemDelegate
{
public:
    CustomIconDelegate(QObject *parent = 0, bool _hideText = false);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    QString displayText(const QVariant &value, const QLocale &locale) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event,
                                       QAbstractItemModel *model,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) override;

    std::function<void(QWidget *, const QModelIndex &) > dataAccessor;
    std::function<void(QWidget *,QAbstractItemModel*,  const QModelIndex &)> dataSetter;
    std::function<QWidget *(QWidget *)> widgetCreator;
    std::function<void (const QStyledItemDelegate* , QPainter *,const QStyleOptionViewItem&,const QModelIndex &)> paintProcessor;
    std::function<bool(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index)> editorEventProcessor;
    bool hideText = false;
};


