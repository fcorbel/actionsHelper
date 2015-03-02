#include <QDebug>
#include "dataObject.h"

DataObject::DataObject(QObject *parent)
    : QObject(parent)
{
}

DataObject::DataObject(const QString &shct, const QString &cont, QObject *parent)
    : QObject(parent), shortcut_(shct), content_(cont)
{
}

QString DataObject::shortcut() const
{
    return shortcut_;
}

QString DataObject::content() const
{
    return content_;
}

