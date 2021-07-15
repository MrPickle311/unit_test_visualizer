#pragma once

#include <Parser.hpp>

using TestResult = uint8_t;
using byte_t     = uint8_t;

class BufferStorage
{
protected:
    port::ByteBuffer* buffer_;
public:
    virtual void setBuffer(port::ByteBuffer* newBuffer);
};

class ByteSender:
        public BufferStorage
{
protected:
    void sendByte(byte_t code);
    void sendBytes(QByteArray bytes);
};

class AbstractUnitTestInjecter:
        public ByteSender
{
private:
    void injectName(const QByteArray& name);
    void injectType(const QByteArray& type);
    void injectCurrentValue(const QByteArray& current_value);
    void injectTestResult(const QByteArray& test_result);
    void injectEnd();
protected:
    virtual void injectValues(const QSharedPointer<UnitTestDataPackage>& unit_test) = 0;
public:
    void inject(const QSharedPointer<UnitTestDataPackage>& unit_test);
};

class UnitTestInjecter:
        public AbstractUnitTestInjecter
{
private:
    void injectExpectedValue(const QByteArray& expected_values);
protected:
    virtual void injectValues(const QSharedPointer<UnitTestDataPackage>& unit_test) override;
};

class RangeUnitTestInjecter:
        public AbstractUnitTestInjecter
{
private:
    void injectLowerValue(const QByteArray& lower_value);
    void injectUpperValue(const QByteArray& upper_value);
protected:
    virtual void injectValues(const QSharedPointer<UnitTestDataPackage>& unit_test) override;
};

class TestCaseInjecter:
        public ByteSender
{
private:
    UnitTestInjecter      unit_injecter_;
    RangeUnitTestInjecter range_injecter_;
private:
    void injectCaseName(const QByteArray& test_case_name);
public:
    void inject(const QSharedPointer<TestCase>& test_case);
    virtual void setBuffer(port::ByteBuffer* newBuffer) override;
};

class TransactionInjecter:
        public ByteSender
{
private:
    TestCaseInjecter case_injecter_;
public:
    virtual void setBuffer(port::ByteBuffer* newBuffer) override;
    void inject(const QSharedPointer<Transaction>& test_case);
};
