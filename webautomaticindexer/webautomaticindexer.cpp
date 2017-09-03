#include "webautomaticindexer.h"

#include <QtNetwork>
#include <QTimer>
#include <QTime>
#include <QDebug>

#include "asynchronous.h"

#define DELAYTIMEMIN 100
#define DELAYTIMEMAX 200

void delayRandTime(){
    static QTime lastUseTime;
    int msec = lastUseTime.msecsTo(QTime::currentTime());
    int delayTime = (DELAYTIMEMAX-DELAYTIMEMIN)*qrand()/RAND_MAX + DELAYTIMEMIN;
    //qDebug() << delayTime;

    if(msec < delayTime){
        Asynchronous delayAsynchronous;
        qDebug() << delayTime << (delayTime - msec);
        delayAsynchronous.start(delayTime - msec);
    }
    lastUseTime = QTime::currentTime();
}

WebAutomaticIndexer::WebAutomaticIndexer(QObject *parent)
    : QObject(parent), manager(new QNetworkAccessManager(this)), autoRedirect(true)
{
    //manager->setCookieJar(new QNetworkCookieJar(manager));
}

QByteArray WebAutomaticIndexer::toHtmlData(QStringList strList)
{
    QByteArray retValue;
    for(int i=1;i<strList.size();i+=2){
        //qDebug() << QUrl::toPercentEncoding(strList.at(i-1)) << QUrl::toPercentEncoding(strList.at(i), "*");
        retValue += "&" + QUrl::toPercentEncoding(strList.at(i-1)) + "=" + QUrl::toPercentEncoding(strList.at(i));
    }

    //remove first '&'
    retValue.remove(0,1);

    return retValue;
}

QByteArray WebAutomaticIndexer::get(QString url, int timeout)
{
    return get(url, QStringList(), timeout);
}

QByteArray WebAutomaticIndexer::get(QString url, QStringList otherHeader, int timeout)
{
    //qDebug() << manager->cookieJar()->cookiesForUrl(url);
    emit print("get Url:" + url);
    lastUrl = url;
    Asynchronous asynchronous;
    QByteArray retValue;

    QNetworkRequest *request = new QNetworkRequest(QUrl(url));
    for(QNetworkCookie cookie : manager->cookieJar()->cookiesForUrl(QUrl(url))){
        request->setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookie));
    }

    for(int i=1;i<otherHeader.size();i+=2)
        request->setRawHeader(QString(otherHeader.at(i-1)).toUtf8(), QString(otherHeader.at(i)).toUtf8());

    QNetworkReply *reply = manager->get(*request);
    connect(reply, &QNetworkReply::downloadProgress, this, &WebAutomaticIndexer::downloadProgress);

    connect(reply, &QNetworkReply::readyRead, this, [=, &retValue, &asynchronous]{
        if(!asynchronous.isRunning())
            return;

        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //qDebug() << "statusCode" << statusCode << url;
        if (statusCode >= 200 && statusCode < 300) {
            retValue += reply->readAll();
        }
    });

    connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, [=](QNetworkReply::NetworkError error){
        qDebug() << error;
    });
    connect(reply, &QNetworkReply::sslErrors, this, [=](QList<QSslError> error){
        qDebug() << error;
    });

    connect(reply, &QNetworkReply::finished, &asynchronous, &Asynchronous::finish);
    asynchronous.start(timeout);
    delete request;
    lastReplyHeaderList = reply->rawHeaderList();
    lastReplyHeaderValueList.clear();
    foreach(QByteArray head, lastReplyHeaderList) {
        lastReplyHeaderValueList << reply->rawHeader(head);
    }

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    emit print("get Url:" + url + ", status code:" + QString::number(statusCode));
    lastStatusCode = statusCode;
    if(statusCode == 302 || statusCode == 301){
        if(autoRedirect == true){
            QString urlLocation = reply->header(QNetworkRequest::LocationHeader).toString();
            emit print("get location to:" + urlLocation);
            //qDebug() << "statusCode:" << statusCode << ", location to:" << urlLocation;
            retValue += get(urlLocation, QStringList(), timeout);
        }
    }
    else{
        delayRandTime();
    }

    reply->deleteLater();

    return retValue;
}

void WebAutomaticIndexer::get2(QString url, QStringList otherHeader, std::function<void (QByteArray)> function)
{
    emit print("get Url:" + url);
    lastUrl = url;
    QByteArray *retValue = new QByteArray;

    QNetworkRequest *request = new QNetworkRequest(QUrl(url));
    for(QNetworkCookie cookie : manager->cookieJar()->cookiesForUrl(QUrl(url))){
        request->setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookie));
    }

    for(int i=1;i<otherHeader.size();i+=2)
        request->setRawHeader(QString(otherHeader.at(i-1)).toUtf8(), QString(otherHeader.at(i)).toUtf8());

    QNetworkReply *reply = manager->get(*request);
    connect(reply, &QNetworkReply::downloadProgress, this, &WebAutomaticIndexer::downloadProgress);

    connect(reply, &QNetworkReply::readyRead, this, [=]{
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //qDebug() << "statusCode" << statusCode << url;
        if (statusCode >= 200 && statusCode < 300) {
            *retValue += reply->readAll();
        }
    });

    connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, [=](QNetworkReply::NetworkError error){
        qDebug() << error;
    });
    connect(reply, &QNetworkReply::sslErrors, this, [=](QList<QSslError> error){
        qDebug() << error;
    });

    connect(reply, &QNetworkReply::finished, [=]{
        function(*retValue);
        delete retValue;

        delete request;
        lastReplyHeaderList = reply->rawHeaderList();
        lastReplyHeaderValueList.clear();
        foreach(QByteArray head, lastReplyHeaderList) {
            lastReplyHeaderValueList << reply->rawHeader(head);
        }

        //delayRandTime();

        reply->deleteLater();
    });
}


QByteArray WebAutomaticIndexer::post(QString url, QByteArray data, QStringList otherHeader, int timeout)
{
    lastUrl = url;
    Asynchronous asynchronous;
    QByteArray retValue;

    QNetworkRequest *request = new QNetworkRequest(QUrl(url));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    for(QNetworkCookie cookie : manager->cookieJar()->cookiesForUrl(QUrl(url))){
        request->setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookie));
    }
    for(int i=1;i<otherHeader.size();i+=2)
        request->setRawHeader(QString(otherHeader.at(i-1)).toUtf8(), QString(otherHeader.at(i)).toUtf8());

    //QNetworkReply *reply = manager->post(*request, QUrl(data).toEncoded());
    QNetworkReply *reply = manager->post(*request, data);

    connect(reply, &QNetworkReply::readyRead, this, [=, &retValue, &asynchronous]{
        if(!asynchronous.isRunning())
            return;

        retValue += reply->readAll();

        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //qDebug() << "statusCode" << statusCode;
        if (statusCode >= 200 && statusCode < 300) {

        }
    });


    connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, [=](QNetworkReply::NetworkError error){
        qDebug() << error;
    });

    connect(reply, &QNetworkReply::finished, &asynchronous, &Asynchronous::finish);
    asynchronous.start(timeout);
    delete request;

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    lastStatusCode = statusCode;
    if(statusCode == 302){
        if(autoRedirect == true){
            QString urlLocation = reply->header(QNetworkRequest::LocationHeader).toString();
            emit print("get location to:" + urlLocation);
            //qDebug() << "location to:" << urlLocation;
            retValue += get(urlLocation, QStringList(), timeout);
        }
    }
    else{
        delayRandTime();
    }

    //qDebug() << "Content-Length" << reply->header(QNetworkRequest::ContentLengthHeader);

    reply->deleteLater();

    return retValue;
}

void WebAutomaticIndexer::post2(QString url, QByteArray data, QStringList otherHeader, std::function<void (QByteArray)> function, int timeout)
{
    lastUrl = url;
    QByteArray *retValue = new QByteArray;

    QNetworkRequest *request = new QNetworkRequest(QUrl(url));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    for(QNetworkCookie cookie : manager->cookieJar()->cookiesForUrl(QUrl(url))){
        request->setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookie));
    }
    for(int i=1;i<otherHeader.size();i+=2)
        request->setRawHeader(QString(otherHeader.at(i-1)).toUtf8(), QString(otherHeader.at(i)).toUtf8());

    //QNetworkReply *reply = manager->post(*request, QUrl(data).toEncoded());
    QNetworkReply *reply = manager->post(*request, data);

    connect(reply, &QNetworkReply::readyRead, this, [=]{
        *retValue += reply->readAll();

        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //qDebug() << "statusCode" << statusCode;
        if (statusCode >= 200 && statusCode < 300) {

        }
    });


    connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, [=](QNetworkReply::NetworkError error){
        qDebug() << error;
    });

    connect(reply, &QNetworkReply::finished, [=]{
        function(*retValue);
        delete retValue;

        delete request;

        reply->deleteLater();
    });
}

QList<QByteArray> WebAutomaticIndexer::getLastReplyHeaderList()
{
    return lastReplyHeaderList;
}

QList<QByteArray> WebAutomaticIndexer::getLastReplyHeaderValueList()
{
    return lastReplyHeaderValueList;
}

QByteArray WebAutomaticIndexer::cookie(QString url)
{
    QByteArray retValue;
    for(QNetworkCookie cookie : manager->cookieJar()->cookiesForUrl(QUrl(url))){
        retValue += cookie.name() + ":" + cookie.value() + "\r\n";
    }

    return retValue;
}

QString WebAutomaticIndexer::getLastUrl()
{
    return lastUrl;
}

int WebAutomaticIndexer::getLastStatusCode()
{
    return lastStatusCode;
}
