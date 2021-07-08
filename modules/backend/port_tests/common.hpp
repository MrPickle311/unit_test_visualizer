#pragma once

#include <QDebug>

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

#define NO_BYTES     0
#define ONE_BYTE     1
#define TWO_BYTES    2
#define THREE_BYTES  3
#define FIVE_BYTES   5
