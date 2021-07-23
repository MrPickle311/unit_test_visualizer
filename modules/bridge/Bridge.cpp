#include "Bridge.hpp"

namespace bridge
{

void Program::makeConnections()
{

}

Program::Program():
    scanner_{QSharedPointer<port::PortScanner>::create()}//inject implementation
{

}


}
