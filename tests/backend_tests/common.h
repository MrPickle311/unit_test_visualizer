#pragma once

#include <QtTest>

class DebugLogger
{
public:
    template<typename DataType>
    static void sendListDataToDebug(QList<DataType>&& data);
};

template<typename DataType>
void DebugLogger::sendListDataToDebug(QList<DataType>&& data)
{
    for(auto&& line : data)
        qDebug() << line;
}
