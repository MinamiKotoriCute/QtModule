#ifndef DEFINEHEADER_H
#define DEFINEHEADER_H

#include <QDebug>

#define PRINT_BUG(var) qDebug() << objectName()+"::"+__func__ << #var+QString("=")+var << __FILE__ << __LINE__ << __DATE__ << __TIME__;
//#define PRINT_BUG(var) fprintf(stderr, "%s::%s %s=\"%s\" [%d %s] %s %s\n", objectName().toLocal8Bit().data(), __func__, #var, var.toLocal8Bit().data(), __LINE__, __FILE__, __DATE__, __TIME__);fflush(stderr);

#include <QObject>

class DefineHeader : public QObject
{
    Q_OBJECT
public:
    explicit DefineHeader(QObject *parent = 0);

signals:

public slots:
};

#endif // DEFINEHEADER_H
