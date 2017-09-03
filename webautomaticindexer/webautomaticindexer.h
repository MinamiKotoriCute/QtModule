#ifndef WEBAUTOMATICINDEXER_H
#define WEBAUTOMATICINDEXER_H

#include <functional>

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QDebug>

class QNetworkAccessManager;

class WebAutomaticIndexer : public QObject
{
    Q_OBJECT
public:
    explicit WebAutomaticIndexer(QObject *parent = 0);

    static QByteArray toHtmlData(QStringList strList);

    QByteArray get(QString url, int timeout = -1);
    QByteArray get(QString url, QStringList otherHeader, int timeout = -1);
    void get2(QString url, QStringList otherHeader, std::function<void (QByteArray)> function);

    QByteArray post(QString url, QByteArray data, QStringList otherHeader = QStringList(), int timeout = -1);
    void post2(QString url, QByteArray data, QStringList otherHeader, std::function<void (QByteArray)> function, int timeout = -1);

    QList<QByteArray> getLastReplyHeaderList();
    QList<QByteArray> getLastReplyHeaderValueList();
    QByteArray cookie(QString url);
    QString getLastUrl();
    int getLastStatusCode();

signals:
    void print(QString);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

public slots:

private:
    bool autoRedirect;
    int lastStatusCode;
    QList<QByteArray> lastReplyHeaderList, lastReplyHeaderValueList;
    QNetworkAccessManager *manager;
    QString lastUrl;
};

#endif // WEBAUTOMATICINDEXER_H
