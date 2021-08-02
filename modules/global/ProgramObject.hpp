#pragma once

#include <QString>
#include <QObject>

namespace global
{

/// @brief This is base class which allows us to check
/// statements at runtime
class ThrowingObject
{
public:
    /// This method throws if passed expression is true.
    /// Throws std::logic_error exception
    /// @param when_throw Expression to check , if its true , method throws
    /// @param msg Message to append to the exception
    void throwIf(bool when_throw , std::string msg) const noexcept(false);
};

/// @brief This class is used to provide basic program functionalites
/// for objects
class ProgramObject:
        public ThrowingObject
{
public:

};

}
