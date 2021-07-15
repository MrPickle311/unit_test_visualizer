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

class UnitTestDataPackage
{
public:
    const QByteArray& getName()          const;
    const QByteArray& getDescriptor()    const;
    const QByteArray& getResult()        const;
    const QByteArray& getCurrentValue()  const;
    const QByteArray& getExpectedValue() const;
    const QByteArray& getLowerValue()    const;
    const QByteArray& getUpperValue()    const;

    void setName(const          QByteArray& newName);
    void setDescriptor(const    QByteArray& newDescriptor);
    void setResult(const        QByteArray& newResult);
    void setCurrentValue(const  QByteArray& newCurrent_value);
    void setExpectedValue(const QByteArray& newExpected_value);
    void setLowerValue(const    QByteArray& newLower_value);
    void setUpperValue(const    QByteArray& newUpper_value);
protected:
    QByteArray  name_;
    QByteArray  descriptor_;
    QByteArray  result_;
    QByteArray  current_value_;
    QByteArray  expected_value_;
    QByteArray  lower_value_;
    QByteArray  upper_value_;
};

class TestCase
{
public:
    using TestPackPtr = QSharedPointer<UnitTestDataPackage>;
private:
    QList<TestPackPtr> tests_;
    QByteArray         test_case_name_;
public:
    const QByteArray& getTestCaseName() const;
    void setTestCaseName(const QByteArray& newTest_case_name);
    void addUnitTest(TestPackPtr test);
    QSharedPointer<UnitTestDataPackage> getUnitTest(int idx);
};

class Transaction
{
private:
    QList<QSharedPointer<TestCase>> cases_;
public:
    void addTestCase(QSharedPointer<TestCase> test_case);
    QSharedPointer<TestCase> getTestCase(int idx);
};

//byte specialized
class ByteStorage
{
protected:
    QByteArray bytes_;//if its composite , bytes_ is empty
public:
    ByteStorage();
    const QByteArray& getBytes() const;
    void setBytes(const QByteArray& newBytes);
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
    ParsedDataPackage& getChild(int idx);
};

