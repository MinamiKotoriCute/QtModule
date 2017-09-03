#include "commonfunction.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>

#define SAVELOADFILENAME "json.txt"
#define SAVELOADFILENAME2 "json2.txt"

static int _save_count = 0;

void save(QString b){
    QFile f(QString::asprintf("aa%d.txt", ++_save_count));
    if(f.open(QFile::WriteOnly)){
        f.write(b.toLocal8Bit().data());
        f.close();
    }
}

void save(QByteArray b){
    QFile f(QString::asprintf("aa%d.txt", ++_save_count));
    if(f.open(QFile::WriteOnly)){
        f.write(b);
        f.close();
    }
}

QJsonObject loadJson(){
    QJsonObject ret;

    QFile file(SAVELOADFILENAME);
    if(file.open(QFile::ReadOnly)){
        ret = QJsonDocument::fromBinaryData(file.readAll()).object();
        file.close();
    }

    return ret;
}

QJsonObject loadJson2(){
    QJsonObject ret;

    QFile file(SAVELOADFILENAME2);
    if(file.open(QFile::ReadOnly)){
        ret = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }

    return ret;
}

void saveJson(QJsonObject json){
    QFile file(SAVELOADFILENAME);
    if(file.open(QFile::WriteOnly)){
        file.write(QJsonDocument(json).toBinaryData());
        file.close();
    }

    QFile file2(SAVELOADFILENAME2);
    if(file2.open(QFile::WriteOnly)){
        file2.write(QJsonDocument(json).toJson(QJsonDocument::Indented));
        file2.close();
    }
}

QString capturedString(const QString &sourse, const QString &pattern)
{
    QRegularExpression re(pattern, QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match = re.match(sourse);
    if (match.hasMatch()) {
        return match.captured(1);
    }
    return QString();
}

void save(QByteArray b, QString fileName)
{
    static int saveCount = 0;
    QFile f(fileName + QString::asprintf("%d.txt", ++saveCount));
    if(f.open(QFile::WriteOnly)){
        f.write(b);
        f.close();
    }
}
