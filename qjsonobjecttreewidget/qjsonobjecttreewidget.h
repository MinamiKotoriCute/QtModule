#ifndef QJSONOBJECTTREEWIDGET_H
#define QJSONOBJECTTREEWIDGET_H

#include <QTreeWidget>
#include <QJsonArray>
#include <QJsonObject>

class QJsonObjectTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QJsonObjectTreeWidget(QWidget *parent = 0);

    void setData(QJsonObject json);
    void setData(QJsonArray json);

signals:

public slots:
};

#endif // QJSONOBJECTTREEWIDGET_H
