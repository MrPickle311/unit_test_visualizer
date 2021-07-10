#pragma once

#include <QString>



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

class ConnectedObject_interface
{
public:
    virtual void makeConnections() = 0;
};

