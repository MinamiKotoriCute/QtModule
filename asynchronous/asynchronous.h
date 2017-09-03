#ifndef ASYNCHRONOUS_H
#define ASYNCHRONOUS_H

#include <QObject>
#include <functional>
#include <QTimer>

class QEventLoop;
class QTimer;

class Asynchronous : public QObject
{
    Q_OBJECT
public:
    explicit Asynchronous(QObject *parent = 0);
    ~Asynchronous();

    void setFuntion(std::function<void ()> funtion);
    void start(int timeout = 0);
    bool isRunning();

signals:

public slots:
    void finish();

private:
    std::function<void ()> pFuntion;
    QEventLoop *loop;
    QTimer *timer;
};

#endif // ASYNCHRONOUS_H
