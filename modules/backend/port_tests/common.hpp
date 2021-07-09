#pragma once

#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QObject>

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


class EventProcessor
{
protected:
    QEventLoop loop_;
protected:
    virtual void startProcessing() = 0;
    virtual void makeConnections() = 0;
    virtual void start()           = 0;
    virtual void showResults()     = 0;
    virtual void stopProcessing()  = 0;
public:
    EventProcessor();
    void waitAndProcessObjectEvent();
};

template<typename TestedObject>
class SignalChecker:
        public EventProcessor
{
public:
    typedef void (*signal_to_test_t)(void);
private:
    QTimer                       timer_;
    QSharedPointer<TestedObject> tested_object_;
    signal_to_test_t             signal_to_test_;
    std::chrono::milliseconds    timeout_;
protected:
    virtual void startProcessing()
    {
        timer_.setSingleShot(true);
    }
    virtual void makeConnections()
    {
        QObject::connect( &timer_, &QTimer::timeout, &loop_, &QEventLoop::quit );
    }
    virtual void start()
    {
        //if(this->signal_to_test_ == nullptr)
        //    throw std::logic_error{"No signal specified!"};
        timer_.start(std::chrono::milliseconds{800});
    }
    virtual void showResults()
    {
        if(timer_.isActive())
            qDebug() << "Signal cought";
        else qDebug() << "Connection lost...";
    }
    virtual void stopProcessing()
    {
        timer_.stop();
    }
public:
    SignalChecker() :
        timer_{},
        tested_object_{nullptr},
        signal_to_test_{nullptr},
        timeout_{5000}
    {}
    //void setObject(QSharedPointer<TestedObject> newObject)
    //{
    //    tested_object_ = newObject;
    //}
    //void setSignalToTest(std::function<void(void)> &bound_signal)
    //{
    //    //QObject::connect(&tested_object_ , signal_to_test_,
    //    //                 &loop_ , &QEventLoop::quit);
    //    this->signal_to_test_ = signal_to_test;
    //}
    QEventLoop* getLoopPtr()// a little workaround , but its temporary
    {
        return &loop_;
    }
};
