#pragma once

#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QObject>
#include "../global/ProgramObject.hpp"

#define NO_BYTES     0
#define ONE_BYTE     1
#define TWO_BYTES    2
#define THREE_BYTES  3
#define FIVE_BYTES   5

class DebugLogger
{
public:
    template<typename DataType>
    static void sendListDataToDebug(QList<DataType>&& data)
    {
        for(auto&& line : data)
            qDebug() << line;
    }
};


class Processor:
        public ConnectedObject_interface
{
protected:
    virtual void startProcessing() = 0;
    virtual void start()           = 0;
    virtual void showResults()     = 0;
    virtual void stopProcessing()  = 0;
public:
    virtual ~Processor(){}
public:
    virtual void waitAndProcessObjectEvent() = 0;
};

class EventProcessor:
        public Processor
{
protected:
    QEventLoop loop_;
public:
    EventProcessor();
    virtual void waitAndProcessObjectEvent();
};

class SignalChecker:
        public QObject,
        public EventProcessor
{
    Q_OBJECT;
private:
    QTimer                       timer_;
    std::chrono::milliseconds    timeout_;
    bool                         is_processing_done_;
protected:
    virtual void startProcessing();
    virtual void makeConnections();
    virtual void start();
    virtual void stopProcessing();
protected slots:
    virtual void showResults();
public:
    SignalChecker();
public:
    void checkEvent();
    void setTimeout(const std::chrono::milliseconds& timeout);
};
