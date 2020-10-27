#include  "custom_icon_delegate.h"
#include <QApplication>
#include <QVariant>


CustomIconDelegate::CustomIconDelegate(QObject *parent, bool _hideText)
    : QStyledItemDelegate(parent), hideText(_hideText)
{
}
QWidget * CustomIconDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    return widgetCreator(parent);
}

void CustomIconDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if(dataAccessor)
        dataAccessor(editor, index);
}
void CustomIconDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if(dataSetter)
        dataSetter(editor, model, index);
}

QString CustomIconDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if(hideText)
        return QString();
    else
        return QStyledItemDelegate::displayText(value, locale);
}


void CustomIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
        return;
    if(paintProcessor)
        paintProcessor(this, painter, option, index);
    else
        QStyledItemDelegate::paint(painter, option, index);
}

bool CustomIconDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(editorEventProcessor)
        return editorEventProcessor(event, model, option, index);
    else
        return QStyledItemDelegate::editorEvent(event, model, option, index);
}
