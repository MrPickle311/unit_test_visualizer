#pragma once

#include <QString>
#include <QObject>

namespace global
{

class ThrowingObject
{
public:
    void throwIf(bool when_throw , std::string msg) const noexcept(false);
};

class ProgramObject:
        public ThrowingObject
{
public:

};

}
