#pragma once

#include <QList>
#include <QString>
#include <variant>
#include "ConvertedDataTypes.hpp"
#include "ParsedDataTypes.hpp"
#include "../global/ProgramObject.hpp"
#include "Interfaces.hpp"

namespace backend
{

class AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const = 0;
};

class Int8NumericValueConverter:
        public AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const override;
};

class Int16NumericValueConverter:
        public AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const override;
};

class Int32NumericValueConverter:
        public AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const override;
};

class Int64NumericValueConverter:
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

using ValueGenerator  =     global::StaticGenerator<TypeDescriptor , QSharedPointer<backend::AbstractValueConverter>>;
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

class Converter:
        public interface::Converter
{
private:
    backend::TransactionDataPackage const * pack_; //cannot change the content
    Transaction                             transaction_;
    TestCaseConverter                       case_converter_;
public:
    Converter();
    virtual Transaction getConvertedTransaction()                   override;
    virtual void        reset()                                     override;
    virtual void        setPack(const TransactionDataPackage* pack) override;
};

}
