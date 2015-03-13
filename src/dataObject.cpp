#include <QDebug>
#include "dataObject.h"

DataObject::DataObject(QObject *parent)
    : QObject(parent)
{
}

DataObject::DataObject(const QString &action, const QString &desc, QObject *parent)
    : QObject(parent), action_(action), description_(desc)
{
}

QString DataObject::action() const
{
    return action_;
}

QString DataObject::description() const
{
    return description_;
}

