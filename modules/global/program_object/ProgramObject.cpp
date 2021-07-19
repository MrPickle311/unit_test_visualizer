#include "../ProgramObject.hpp"
#include <QtDebug>


void ThrowingObject::throwIf(bool when_throw, std::string msg) const
{
    if(when_throw)
        throw std::logic_error {msg};
}
