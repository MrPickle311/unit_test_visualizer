#pragma once

#include <QList>
#include <QSharedPointer>
#include <memory>

//class Composite
//{
//public:
//    virtual bool       isLeaf()                        = 0 ;//if its a leaf -> contains bytes
//    virtual void       setIsLeaf(bool newIs_leaf)      = 0 ;
//    virtual Composite* getParent()                     = 0 ;
//    virtual void       setParent(Composite* newParent) = 0 ;
//    virtual void       addChildren(Composite* child)   = 0 ;
//};


//byte specialized
class ByteStorage
{
protected:
    QByteArray bytes_;//if its composite , bytes_ is empty
public:
    ByteStorage();
    const QByteArray& getbytes() const;
    void setbytes(const QByteArray& newBytes);
};

class ParsedDataPackage:
        public ByteStorage
{
private:
    QSharedPointer<ParsedDataPackage>        parent_;
    bool                                     is_leaf_;//if its true -> end of processing
    QList<QSharedPointer<ParsedDataPackage>> children_;
public:
    ParsedDataPackage(QSharedPointer<ParsedDataPackage> parent = nullptr);
    bool isLeaf();//if its a leaf -> contains bytes
    void setIsLeaf(bool newIs_leaf);

    QSharedPointer<ParsedDataPackage> getParent();
    void setParent(QSharedPointer<ParsedDataPackage> newParent);

    void addChild(QSharedPointer<ParsedDataPackage> child);
};

