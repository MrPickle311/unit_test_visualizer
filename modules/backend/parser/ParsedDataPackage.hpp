#pragma once

#include <QList>
#include <QSharedPointer>
#include <memory>

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
public:
    bool operator==(const UnitTestDataPackage& other) const
    {
        if(this->name_ != other.name_)
            return false;

        if(this->descriptor_ != other.descriptor_)
            return false;

        if(this->result_ != other.result_)
            return false;

        if(this->current_value_ != other.current_value_)
            return false;

        if(this->expected_value_ != other.expected_value_)
            return false;

        if(this->lower_value_ != other.lower_value_)
            return false;

        if(this->upper_value_ != other.upper_value_)
            return false;

        return true;
    }
    bool operator!=(const UnitTestDataPackage& other) const
    {
        return !(*this == other);
    }
};

class TestCaseDataPackage
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
    const QList<TestPackPtr>& getTests() const;
    bool operator==(const TestCaseDataPackage& other)
    {
        if(this->tests_.size() != other.tests_.size())
            return false;

        if(this->test_case_name_ != other.test_case_name_)
            return false;

        for(int i{0} ; i < tests_.size() ; ++i)
            if(*this->tests_[i] != *other.tests_[i])
                return false;

        return true;
    }
    bool operator!=(const TestCaseDataPackage& other)
    {
        return !(*this == other);
    }
};

class TransactionDataPackage
{
private:
    QList<QSharedPointer<TestCaseDataPackage>> cases_;
public:
    void addTestCase(QSharedPointer<TestCaseDataPackage> test_case);
    QSharedPointer<TestCaseDataPackage> getTestCase(int idx);
    const QList<QSharedPointer<TestCaseDataPackage> >& getCases() const;
    bool operator==(const TransactionDataPackage& other)
    {
        if(this->cases_.size() != other.cases_.size())
            return false;

        for(int i{0} ; i < cases_.size() ; ++i)
            if(*this->cases_[i] != *other.cases_[i])
                return false;

        return true;
    }
};
