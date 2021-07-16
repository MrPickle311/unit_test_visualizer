#include "ParsedDataPackage.hpp"

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

const QList<QSharedPointer<UnitTestDataPackage>>& TestCaseDataPackage::getTests() const
{
    return tests_;
}

const QByteArray& TestCaseDataPackage::getTestCaseName() const
{
    return test_case_name_;
}

void TestCaseDataPackage::setTestCaseName(const QByteArray& newTest_case_name)
{
    test_case_name_ = newTest_case_name;
}

void TestCaseDataPackage::addUnitTest(TestPackPtr test)
{
    tests_.append(test);
}

QSharedPointer<UnitTestDataPackage> TestCaseDataPackage::getUnitTest(int idx)
{
    return tests_[idx];
}

const QList<QSharedPointer<TestCaseDataPackage> >& TransactionDataPackage::getCases() const
{
    return cases_;
}

void TransactionDataPackage::addTestCase(QSharedPointer<TestCaseDataPackage> test_case)
{
    cases_.append(test_case);
}

QSharedPointer<TestCaseDataPackage> TransactionDataPackage::getTestCase(int idx)
{
    return cases_[idx];
}
