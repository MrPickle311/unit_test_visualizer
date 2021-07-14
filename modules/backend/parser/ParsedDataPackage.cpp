#include "ParsedDataPackage.hpp"

const QByteArray& ByteStorage::getbytes() const
{
    return bytes_;
}

void ByteStorage::setbytes(const QByteArray& newBytes)
{
    bytes_ = newBytes;
}

///

bool ParsedDataPackage::isLeaf()
{
    return is_leaf_;
}

void ParsedDataPackage::setIsLeaf(bool newIs_leaf)
{
    is_leaf_ = newIs_leaf;
}

QSharedPointer<ParsedDataPackage> ParsedDataPackage::getParent()
{
    return parent_;
}

void ParsedDataPackage::setParent(QSharedPointer<ParsedDataPackage> newParent)
{
    parent_.reset(newParent.data());
}

void ParsedDataPackage::addChildren(QSharedPointer<ParsedDataPackage> child)
{
    children_.push_back(child);
}


ByteStorage::ByteStorage()
    : bytes_{}
{  }
