#pragma once

#include <QMutex>
#include <QMap>

namespace global
{

template<typename KeyType , typename ValueType>
class StaticGenerator
{
private:
    static QMutex mutex_;
    static QMap< KeyType , ValueType > variables_;
    static bool is_initialized_;
private:
    static void addValue(KeyType descriptor , ValueType value)
    {
        variables_[descriptor] = value;
    }
    static void initValues();//requires user implementation
protected:
    StaticGenerator(){};
public:
    static const ValueType getValue(KeyType key)
    {
        QMutexLocker{&mutex_};
        if(not is_initialized_)
            initValues();
        return variables_[key];
    }
};

template<typename KeyType , typename ValueType>
QMutex StaticGenerator<KeyType, ValueType>::mutex_;

template<typename KeyType , typename ValueType>
QMap<KeyType , ValueType > StaticGenerator<KeyType , ValueType>::variables_;

template<typename KeyType , typename ValueType>
bool StaticGenerator<KeyType , ValueType>::is_initialized_{false};

}

