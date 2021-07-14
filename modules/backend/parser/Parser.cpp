#include "../Parser.hpp"
#include <algorithm>

namespace parser
{

//Sizes singleton initialization

QMutex TypesSizes::mutex_{};
QMap<TypeDescriptor , int> TypesSizes::sizes_{};
bool TypesSizes::sizes_initialized_{false};

void TypesSizes::addSize(TypeDescriptor desc , int size)
{
    sizes_[desc] = size;
}

void TypesSizes::initSizes()
{
    addSize(TypeDescriptor::BIT, 1);
    addSize(TypeDescriptor::BOOL, 1);
    addSize(TypeDescriptor::CHAR, 1);
    addSize(TypeDescriptor::PTR, 1);
    addSize(TypeDescriptor::UINT8_T, 1);
    addSize(TypeDescriptor::INT8_T, 1);

    addSize(TypeDescriptor::INT16_T, 2);
    addSize(TypeDescriptor::UINT16_T, 2);

    addSize(TypeDescriptor::INT32_T, 4);
    addSize(TypeDescriptor::UINT32_T, 4);

    addSize(TypeDescriptor::INT64_T, 8);
    addSize(TypeDescriptor::UINT64_T, 8);

    sizes_initialized_ = true;
}

int TypesSizes::getSize(TypeDescriptor desc)
{
    QMutexLocker{&mutex_};
    if(not sizes_initialized_)
        initSizes();
    return sizes_[desc];
}

TypeDescriptor  ParserComponent::current_type_{};

}



