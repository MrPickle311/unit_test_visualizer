#pragma once

#include <QStringList>

namespace global
{

template<typename... Args>
QStringList joinStringListElements(Args... args)
{
    QStringList result;

    bool isBegin{true};

    auto appender = [&](QStringList list){
        for(int i{0} ; i < list.size() ; ++i)
        {
            if(isBegin)//its first invocation of appender(args) , allocate the memory for nodes
                result.push_back({});
            result[i] += " " + list[i];
        }

        isBegin = false;
    };

    (appender(args), ...);

    return result;
}

}
