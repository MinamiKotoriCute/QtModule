#include "regularexpression.h"

#include <QRegularExpression>

RegularExpression::RegularExpression(QObject *parent)
    : QObject(parent)
{

}

QStringList RegularExpression::simplified(const QStringList &strList)
{
    QStringList newStrList;
    foreach(QString str, strList){
        newStrList << str.simplified();
    }
    return newStrList;
}

QList<QStringList> RegularExpression::simplified(const QList<QStringList> &strLL)
{
    QList<QStringList> newStrLL;
    foreach(QStringList strList, strLL){
        newStrLL << simplified(strList);
    }
    return newStrLL;
}

QList<QStringList> RegularExpression::match(QString pattern, QString string)
{
    QList<QStringList> retValue;
    QRegularExpression mainRE(pattern);
    mainRE.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);

    QRegularExpressionMatchIterator mainIt = mainRE.globalMatch(string);
    while(mainIt.hasNext()){
        QRegularExpressionMatch mainMatch = mainIt.next();
        QStringList strList = mainMatch.capturedTexts();
        if(!strList.isEmpty())
            strList.removeFirst();
        retValue.push_back(strList);
    }

    return retValue;
}
