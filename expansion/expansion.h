#ifndef EXPANSION_H
#define EXPANSION_H

#include <QObject>

QByteArray _read(QString fileName);
void _save(QByteArray b);
QString _split(QString source, QString sep, int number, int min = -1);

class Expansion : public QObject
{
    Q_OBJECT
public:
    explicit Expansion(QObject *parent = 0);

signals:

public slots:
};

#endif // EXPANSION_H
