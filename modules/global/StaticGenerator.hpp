#pragma once

#include <QMutex>
#include <QMap>

namespace global
{

/// @brief This class template allows to create singleton which
/// stores static collection of object.
///
/// Objects are mapped , so access to them is possible by a key.
/// To create singleton user has to specialize initValues()
/// method and put there certain objects. Objects are binded
/// by key-value map ,so this template has two parameters:
/// @param KeyType Type of keys in the map
/// @param ValueType Value of keys in the map
///
/// This class is threadsafe
template<typename KeyType , typename ValueType>
class StaticGenerator
{
private:
    static QMutex                      mutex_;
    static QMap< KeyType , ValueType > variables_;
    static bool                        is_initialized_;
private:
    /// This method is used to add value to the map
    /// @param key Key of the value
    /// @param value Object or value
    static void addValue(KeyType key , ValueType value)
    {
        variables_[key] = value;
    }
    ///This method has to be specialized by user.
    ///User has to add desired objects to the map
    static void initValues();//requires user implementation
protected:
    StaticGenerator(){};
public:
    /// This method is used to get value copy from the map.
    /// @param key Key to which object/value is binded
    static const ValueType getValue(KeyType key)
    {
        QMutexLocker{&mutex_};
        if(not is_initialized_)
        {
            initValues();
            is_initialized_ = true;
        }
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

