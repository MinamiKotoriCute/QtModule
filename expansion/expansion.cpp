#include "expansion.h"

#include <QFile>

QByteArray _read(QString fileName){
    QByteArray b;
    QFile f(fileName);
    if(f.open(QFile::ReadOnly)){
        b = f.readAll();
        f.close();
    }
    return b;
}

void _save(QByteArray b){
    static int i = 0;
    QFile f(QString::asprintf("%d.txt", ++i));
    if(f.open(QFile::WriteOnly)){
        f.write(b);
        f.close();
    }
}

QString _split(QString source, QString sep, int number, int min){
    QStringList strList = source.split(sep);
    if(number == -1){
        //find first
        if(strList.size() >= min)
            return strList.first();
    }
    else if(number == -2){
        //find end
        if(strList.size() >= min)
            return strList.last();
    }
    /*else if(number == -11){
        //find except first
        if(strList.size() >= min)
            return strList.first();
    }*/
    else if(number == -12){
        //find except end
        for(int i=0;i<min;i++)
            if(!strList.isEmpty())
                strList.removeLast();
        QString str;
        for(int i=0;i<strList.size();i++)
            str+=strList.at(i) + sep;
        if(!strList.isEmpty())
            str.remove(str.size()-sep.size(), sep.size());
        return str;
    }
    else if(strList.size() > number){
        return strList.at(number);
    }
    return QString();
}


Expansion::Expansion(QObject *parent)
    : QObject(parent)
{

}
