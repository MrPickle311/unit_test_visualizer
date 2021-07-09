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


