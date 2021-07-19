#include "Bridge.hpp"

Program::Program()
{

}

void Printer::print()
{
    qDebug() << buf_->getAllBytes();
}

SingletonInterface::SingletonInterface(QObject* parent):
    QObject{parent},
    some_property{9}
{

}

int SingletonInterface::getSomeProperty() const
{
    return some_property;
}

void SingletonInterface::setSomeProperty(int newSomeProperty)
{
    if (some_property == newSomeProperty)
        return;
    some_property = newSomeProperty;
    emit somePropertyChanged();
}

void SingletonInterface::changeProperty()
{
    setSomeProperty(5);
}
