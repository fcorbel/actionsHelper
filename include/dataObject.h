#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QObject>

class DataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString action READ action CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)

public:
    DataObject(QObject *parent=0);
    DataObject(const QString &shct, const QString &cont, QObject *parent=0);

    QString action() const;
    QString description() const;

private:
    QString action_;
    QString description_;
};

#endif // DATAOBJECT_H
