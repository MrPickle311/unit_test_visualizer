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


class Converter
{
private:
    const Transaction& transaction_;
public:
    void setTarget(const TransactionDataPackage& pack);
    Transaction getConverterTransaction();
};
