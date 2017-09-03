#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H

#include <QObject>

void save(QString b);
void save(QByteArray b);
void save(QByteArray b, QString fileName);
QJsonObject loadJson();
QJsonObject loadJson2();
void saveJson(QJsonObject json);
void saveJson(QJsonArray json);
QString capturedString(const QString &sourse, const QString &pattern);

#endif // COMMONFUNCTION_H
