#include "asynchronous.h"

#include <QTimer>
#include <QEventLoop>
#include <QDebug>

#define PRINT(text)// qDebug() << text;

Asynchronous::Asynchronous(QObject *parent)
    : QObject(parent),
      loop(new QEventLoop(this)),
      timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, timer, &QTimer::stop);
    connect(timer, &QTimer::timeout, loop, &QEventLoop::quit);
}

Asynchronous::~Asynchronous()
{
    PRINT("~Asynchronous()")
}

void Asynchronous::setFuntion(std::function<void ()> funtion)
{
    pFuntion = funtion;
}

void Asynchronous::start(int timeout)
{
    if(timeout >= 0){
        timer->start(timeout);
    }

    if(pFuntion != nullptr)
        pFuntion();

    loop->exec();
    PRINT("exit")
}

bool Asynchronous::isRunning()
{
    return loop->isRunning();
}

void Asynchronous::finish()
{
    QTimer::singleShot(0, [=]{
        loop->exit();
    });
}
