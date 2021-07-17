#pragma once

#include <QList>
#include <QString>
#include <variant>
#include "Parser.hpp"
#include <map>

struct UnitTest
{
    QString   type_descriptor_;
    QString   name_;
    QString   expecteted_value_;
    QString   current_value_;
    QString   lower_value_;
    QString   upper_value_;
    QString   test_result_;
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

class ValueConverters
{
private:
    static QMutex mutex_;
    static QMap<parser::TypeDescriptor , QSharedPointer<AbstractValueConverter> > value_converters_;
    static bool value_converters_initialized_;
private:
    static void addConverter(parser::TypeDescriptor desc , QSharedPointer<AbstractValueConverter> value_converters);
    static void initValueConverters();
protected:
    ValueConverters(){};
public:
    static const AbstractValueConverter& getConverter(parser::TypeDescriptor desc) ;
};

template<typename T>
class Generator//TODO
{
private:
    static QMutex mutex_;
    static QMap<parser::TypeDescriptor , T > variables_;
    static bool is_initialized_;
private:
    static void addValue(parser::TypeDescriptor descriptor , T value)
    {

    }
    static void initValues();
protected:
    Generator(){};
public:
    static const T getValue(parser::TypeDescriptor desc)
    {
        QMutexLocker{&mutex_};
        if(not is_initialized_)
            initValues();
        return variables_[desc];
    }
};

template<typename T>
QMutex Generator<T>::mutex_;

template<typename T>
QMap<parser::TypeDescriptor , T > Generator<T>::variables_;

template<typename T>
bool Generator<T>::is_initialized_{false};

class UnitTestConverter:
        public ProgramObject
{
protected:
    void                      convertValues(UnitTest& test , const QSharedPointer<UnitTestDataPackage>& pack);
    parser::TypeDescriptor    getDescriptor(const QSharedPointer<UnitTestDataPackage>& test);
    QString                   getTestResult(const QSharedPointer<UnitTestDataPackage>& test);
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
    Transaction getConverterTransaction();
};
