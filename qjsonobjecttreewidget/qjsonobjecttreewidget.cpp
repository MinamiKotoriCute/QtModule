#include "qjsonobjecttreewidget.h"

void listJsonArray(QTreeWidgetItem *parentWidgetItem, QJsonArray &json);
void listJsonObject(QTreeWidgetItem *parentWidgetItem, QJsonObject &json);

void listJsonArray(QTreeWidgetItem *parentWidgetItem, QJsonArray &json) {
    for(int i=0;i<json.size();i++){
        QJsonValue value = json.at(i);
        if(value.isArray()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({QString::number(i+1)}));
            parentWidgetItem->addChild(child);
            listJsonArray(child, value.toArray());
        }
        else if(value.isBool()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({QString::number(i+1), value.toBool() ? "true" : "false"}));
            parentWidgetItem->addChild(child);
        }
        else if(value.isDouble()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({QString::number(i+1), QString::number(value.toDouble())}));
            parentWidgetItem->addChild(child);
        }
        else if(value.isObject()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({QString::number(i+1)}));
            parentWidgetItem->addChild(child);
            listJsonObject(child, value.toObject());
        }
        else if(value.isString()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({QString::number(i+1), value.toString()}));
            parentWidgetItem->addChild(child);
        }
    }
}

void listJsonObject(QTreeWidgetItem *parentWidgetItem, QJsonObject &json) {
    const QStringList keysList = json.keys();

    for(const QString key : keysList){
        const QJsonValue value = json.value(key);
        if(value.isArray()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({key}));
            parentWidgetItem->addChild(child);
            listJsonArray(child, value.toArray());
        }
        else if(value.isBool()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({key, value.toBool() ? "true" : "false"}));
            parentWidgetItem->addChild(child);
        }
        else if(value.isDouble()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({key, QString::number(value.toDouble())}));
            parentWidgetItem->addChild(child);
        }
        else if(value.isObject()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({key}));
            parentWidgetItem->addChild(child);
            listJsonObject(child, value.toObject());
        }
        else if(value.isString()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({key, value.toString()}));
            parentWidgetItem->addChild(child);
        }
    }
}

QJsonObjectTreeWidget::QJsonObjectTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{

}

void QJsonObjectTreeWidget::setData(QJsonObject json)
{
    clear();

    setHeaderLabels(QStringList({"key", "value"}));

    QTreeWidgetItem *top = new QTreeWidgetItem;
    addTopLevelItem(top);

    listJsonObject(top, json);
}

void QJsonObjectTreeWidget::setData(QJsonArray json)
{
    clear();

    setHeaderLabels(QStringList({"key", "value"}));

    QTreeWidgetItem *top = new QTreeWidgetItem;
    addTopLevelItem(top);

    listJsonArray(top, json);
}
