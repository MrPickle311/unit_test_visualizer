#include "../ProgramObject.hpp"
#include <QtDebug>

namespace global
{

void ThrowingObject::throwIf(bool when_throw, std::string msg) const
{
    if(when_throw)
        throw std::logic_error {msg};
}

}
