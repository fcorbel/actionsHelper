#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QObject>

class DataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString shortcut READ shortcut CONSTANT)
    Q_PROPERTY(QString content READ content CONSTANT)

public:
    DataObject(QObject *parent=0);
    DataObject(const QString &shct, const QString &cont, QObject *parent=0);

    QString shortcut() const;
    QString content() const;

private:
    QString shortcut_;
    QString content_;
};

#endif // DATAOBJECT_H
