#pragma once

#include <QString>

class ThrowingObject
{
public:
    void throwIf(bool when_throw , std::string msg);
};

class ProgramObject:
        public ThrowingObject
{
public:

};
