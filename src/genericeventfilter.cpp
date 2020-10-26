#include "include/genericeventfilter.h"
GenericEventFilter::GenericEventFilter(QObject* parent):QObject(parent)
{

}
GenericEventFilter::~GenericEventFilter()
{}

bool GenericEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(eventProcessor != nullptr && eventProcessor(obj, event))
    {
         event->accept();
         return true;
    }
    return QObject::eventFilter(obj, event);
}
void GenericEventFilter::SetEventProcessor(std::function<bool(QObject *, QEvent *)> processor)
{
    eventProcessor = processor;
}
