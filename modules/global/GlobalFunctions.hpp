#pragma once

#include <QStringList>
namespace global
{

/// This template function merges several string lists
/// @param args String lists to merge
/// @return Concated string list
/// ### Example
/// ~~~~~~~~~~~~~~~~~~~~~.cpp
/// QStringList l1{"abc , "def"};
/// QStringList l2{"ABC , "DEF:};
/// auto l3 {joinStringListElements(l1,l2)};
/// //l3 == { "abc ABC" , "def DEF" }
/// ~~~~~~~~~~~~~~~~~~~~
/// @warning Passed string lists have to be same length!
template<typename... Args>
QStringList joinStringListElements(Args... args)
{
    QStringList result;

    bool isBegin{true};

    auto appender = [&](QStringList list){
        for(int i{0} ; i < list.size() ; ++i)
        {
            if(isBegin)//if its first invocation of appender(args) -> allocate the memory for nodes
                result.push_back({});
            result[i] += " " + list[i];// appends also space
        }

        isBegin = false;
    };

    (appender(args), ...);//execute algorithm

    return result;
}

}
