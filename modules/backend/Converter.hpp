#pragma once

#include <QList>
#include <QString>
#include <variant>
#include "Parser.hpp"
#include "../global/StaticGenerator.hpp"

namespace backend
{

struct UnitTest
{
    Q_GADGET;
    Q_PROPERTY(QString typeDescriptor   MEMBER  type_descriptor_ )
    Q_PROPERTY(QString name             MEMBER  name_            )
    Q_PROPERTY(QString expectetedValue  MEMBER  expecteted_value_)
    Q_PROPERTY(QString currentValue     MEMBER  current_value_   )
    Q_PROPERTY(QString lowerValue       MEMBER  lower_value_     )
    Q_PROPERTY(QString upperValue       MEMBER  upper_value_     )
    Q_PROPERTY(QString testResult       MEMBER  test_result_     )
public:
    QString   type_descriptor_;
    QString   name_;
    QString   expecteted_value_;
    QString   current_value_;
    QString   lower_value_;
    QString   upper_value_;
    QString   test_result_;
};

}

//this macro requires to be placed id the global namespace
Q_DECLARE_METATYPE(backend::UnitTest);

namespace backend
{

struct TestCase
{
    QList<UnitTest> tests_;
    QString         test_case_name_;
};

struct Transaction
{
    QList<TestCase> cases_;
};

class AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const = 0;
};

class SignedNumericValueConverter:
        public AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const override;
};

class UnsignedNumericValueConverter:
        public AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const override;
};

class BoolValueConverter:
        public AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const override;
};

class PtrValueConverter:
        public AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const override;
};

class BitValueConverter:
        public AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const override;
};

class CharValueConverter:
        public AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const override;
};

using ValueGenerator = global::StaticGenerator<TypeDescriptor , QSharedPointer<backend::AbstractValueConverter>>;
using StringGenerator = global::StaticGenerator<TypeDescriptor , QString>;

}

namespace global
{

template<>
void backend::ValueGenerator::initValues();

template<>
void backend::StringGenerator::initValues();

}

namespace backend
{

class UnitTestConverter:
        public global::ProgramObject
{
protected:
    void              convertValues(UnitTest& test , const QSharedPointer<UnitTestDataPackage>& pack);
    TypeDescriptor    getDescriptor(const QSharedPointer<UnitTestDataPackage>& test);
    QString           getTestResult(const QSharedPointer<UnitTestDataPackage>& test);
public:
    UnitTestConverter();
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
    Transaction getConvertedTransaction();
    void reset();
};

}
