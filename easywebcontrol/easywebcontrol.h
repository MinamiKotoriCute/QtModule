#ifndef EASYWEBCONTROL_H
#define EASYWEBCONTROL_H

#include <QObject>
#include <QTextCodec>

#include "webautomaticindexer.h"

QString urlFix(QString url);

class EasyWebControl : public QObject
{
    Q_OBJECT
public:
    explicit EasyWebControl(QObject *parent = 0);

signals:

public slots:

protected:
    WebAutomaticIndexer *web;
    QTextCodec *codec;
};

#endif // EASYWEBCONTROL_H
