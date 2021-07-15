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

const QByteArray& UnitTestDataPackage::getName() const
{
    return name_;
}

const QByteArray& UnitTestDataPackage::getDescriptor() const
{
    return  descriptor_;
}

const QByteArray& UnitTestDataPackage::getResult() const
{
    return result_;
}

const QByteArray& UnitTestDataPackage::getCurrentValue() const
{
    return  current_value_;
}

const QByteArray& UnitTestDataPackage::getExpectedValue() const
{
    return  expected_value_;
}

const QByteArray& UnitTestDataPackage::getLowerValue() const
{
    return lower_value_;
}

const QByteArray& UnitTestDataPackage::getUpperValue() const
{
    return  upper_value_;
}

void UnitTestDataPackage::setName(const QByteArray& newName)
{
    name_ = newName;
}

void UnitTestDataPackage::setDescriptor(const QByteArray& newDescriptor)
{
    descriptor_ = newDescriptor;
}

void UnitTestDataPackage::setResult(const QByteArray& newResult)
{
    result_ = newResult;
}

void UnitTestDataPackage::setCurrentValue(const QByteArray& newCurrent_value)
{
    current_value_ = newCurrent_value;
}

void UnitTestDataPackage::setExpectedValue(const QByteArray& newExpected_value)
{
    expected_value_ = newExpected_value;
}

void UnitTestDataPackage::setLowerValue(const QByteArray& newLower_value)
{
    lower_value_ = newLower_value;
}

void UnitTestDataPackage::setUpperValue(const QByteArray& newUpper_value)
{
    upper_value_ = newUpper_value;
}

const QByteArray& TestCase::getTestCaseName() const
{
    return test_case_name_;
}

void TestCase::setTestCaseName(const QByteArray& newTest_case_name)
{
    test_case_name_ = newTest_case_name;
}

void TestCase::addUnitTest(TestPackPtr test)
{
    tests_.append(test);
}

QSharedPointer<UnitTestDataPackage> TestCase::getUnitTest(int idx)
{
    return tests_[idx];
}

void Transaction::addTestCase(QSharedPointer<TestCase> test_case)
{
    cases_.append(test_case);
}

QSharedPointer<TestCase> Transaction::getTestCase(int idx)
{
    return cases_[idx];
}
