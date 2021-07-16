#pragma once

#include <QList>
#include <QString>
#include <variant>
#include <Parser.hpp>

using ValueType = std::variant<int , uint , std::monostate>;

struct UnitTest
{
    uint8_t   type_descriptor_;
    QString   name_;
    ValueType expecteted_value_;
    ValueType current_value_;
    ValueType lower_value_;
    ValueType upper_value_;
    bool      test_result_;
};

struct TestCase
{
    QList<UnitTest> tests_;
    QString         test_case_name_;
};

struct Transaction
{
    QList<TestCase> cases_;
};

class NumericValueConverter
{
public:
    ValueType getNumericValue(const QByteArray& bytes);
};

class UnitTestConverter:
        public ProgramObject
{
protected:
    NumericValueConverter numeric_converter_;
protected:
    uint8_t getDescriptor(const QSharedPointer<UnitTestDataPackage>& test);
    bool    getTestResult(const QSharedPointer<UnitTestDataPackage>& test);
public:
    UnitTest getUnitTest(const QSharedPointer<UnitTestDataPackage>& test);
};

class TestCaseConverter
{
private:
    UnitTestConverter test_converter_;
public:
    TestCaseConverter();
    TestCase getTestCase(const QSharedPointer<TestCaseDataPackage>& test_case);
};


class Converter
{
private:
    Transaction transaction_;
    const TransactionDataPackage& pack_;
    TestCaseConverter case_converter_;
public:
    Converter(const TransactionDataPackage& pack);
    Transaction getConverterTransaction();
};
