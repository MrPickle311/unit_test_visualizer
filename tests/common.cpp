#include "common.hpp"

EventProcessor::EventProcessor():
    loop_{}
{}

void EventProcessor::waitAndProcessObjectEvent()
{
    startProcessing();

    makeConnections();

    start();

    loop_.exec();
}

void SignalChecker::setTimeout(const std::chrono::milliseconds& timeout)
{
    timeout_ = timeout;
}

void SignalChecker::startProcessing()
{
    timer_.setSingleShot(true);
}

void SignalChecker::makeConnections()
{
    QObject::connect( &timer_, &QTimer::timeout, this , &SignalChecker::showResults);
}

void SignalChecker::start()
{
    timer_.start(timeout_);
}

void SignalChecker::showResults()
{
    if(timer_.isActive())
        qDebug() << "Signal cought";
    else qDebug() << "Timeout ...";
}

void SignalChecker::stopProcessing()
{
    is_processing_done_ = true;
    timer_.stop();
}

SignalChecker::SignalChecker() :
    timer_{},
    timeout_{800},
    is_processing_done_{false}
{}

void SignalChecker::checkEvent()
{
    loop_.quit();
    if(!is_processing_done_)
        showResults();
    stopProcessing();
}
