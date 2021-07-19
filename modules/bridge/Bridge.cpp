#include "Bridge.hpp"

Program::Program()
{

}

void Printer::print()
{
    qDebug() << buf_->getAllBytes();
}
