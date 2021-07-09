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

    showResults();

    stopProcessing();
}

