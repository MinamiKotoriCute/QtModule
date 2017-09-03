#include "easywebcontrol.h"

#include <QTextCodec>
#include <QRegularExpression>

QString urlFix(QString url)
{
    for(int i=32;i<127;i++){
        //url.replace("&#([0-9]{2,3});", QChar(QString("\\1").toInt()));
        url.replace(QRegularExpression(QString::asprintf("&#%d;", i)), QString(char(i)));
    }
    return url.replace("amp;", "");
}

EasyWebControl::EasyWebControl(QObject *parent)
    : QObject(parent), web(new WebAutomaticIndexer), codec(QTextCodec::codecForName("UTF-8"))
{

}
