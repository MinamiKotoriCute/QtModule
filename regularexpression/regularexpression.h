#ifndef REGULAREXPRESSION_H
#define REGULAREXPRESSION_H

#include <QObject>

class RegularExpression : public QObject
{
    Q_OBJECT
public:
    explicit RegularExpression(QObject *parent = 0);

    static QStringList simplified(const QStringList &strList);
    static QList<QStringList> simplified(const QList<QStringList> &strLL);
    static QList<QStringList> match(QString pattern, QString string);

signals:

public slots:

private:
};

#endif // REGULAREXPRESSION_H
