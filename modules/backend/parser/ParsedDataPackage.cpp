#include "ParsedDataPackage.hpp"

const QByteArray& ByteStorage::getBytes() const
{
    return bytes_;
}

void ByteStorage::setBytes(const QByteArray& newBytes)
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

void ParsedDataPackage::addChild(QSharedPointer<ParsedDataPackage> child)
{
    children_.push_back(child);
}

ParsedDataPackage& ParsedDataPackage::getChild(int idx)
{
    return *children_.at(idx);
}


ByteStorage::ByteStorage()
    : bytes_{}
{  }

ParsedDataPackage::ParsedDataPackage(QSharedPointer<ParsedDataPackage> parent) :
    parent_{parent},
    is_leaf_{false},
    children_{}
{}
