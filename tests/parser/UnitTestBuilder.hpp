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

class AbstractUnitTestDataPackage:
        public BufferStorage
{
private:
    void injectName();
    void injectType();
    void injectCurrentValue();
    void injectTestResult();
    void injectEnd();
protected:
    void injectMultiValue(QList<uint8_t> values);
    virtual void injectValues() = 0;
public:
    const std::string& getName() const;
    void setName(const std::string& newName);
    parser::TypeDescriptor getDescriptor() const;
    void setDescriptor(parser::TypeDescriptor newDescriptor);
    TestResult getResult() const;
    void setResult(TestResult newResult);
    const QList<byte_t>& getCurrentValue() const;
    void setCurrentValue(const QList<byte_t>& newCurrent_value);
protected:
    std::string             name_;
    parser::TypeDescriptor  descriptor_;
    TestResult              result_;
    QList<byte_t>           current_value_;
public:
    void inject();
};

class UnitTestDataPackage:
        public AbstractUnitTestDataPackage
{
private:
    void appendExpectedValue();
protected:
    virtual void injectValues();
public:
    const QList<uint8_t>& getExpectedValue() const;
    void setExpectedValue(const QList<uint8_t>& newExpected_value);
protected:
    QList<uint8_t> expected_value_;
};

class RangeUnitTestDataPackage:
        public AbstractUnitTestDataPackage
{
private:
    void injectLowerValue();
    void injectUpperValue();
protected:
    virtual void injectValues();
public:
    const QList<uint8_t>& getLowerValue() const;
    void setLowerValue(const QList<uint8_t>& newLower_value);
    const QList<uint8_t>& getUpperValue() const;
    void setUpperValue(const QList<uint8_t>& newUpper_value);
protected:
    QList<uint8_t> lower_value_;
    QList<uint8_t> upper_value_;
};

class TestCase:
        public BufferStorage
{
public:
    using TestPackPtr = QSharedPointer<AbstractUnitTestDataPackage>;
private:
    QList<TestPackPtr> tests_;
    std::string        test_case_name_;
private:
    void injectCaseName();
public:
    void inject();
    const std::string& getTestCaseName() const;
    void setTestCaseName(const std::string& newTest_case_name);
    void addUnitTest(TestPackPtr test);
public:
    virtual void setBuffer(port::ByteBuffer* newBuffer) override;
};

class Transaction:
        public BufferStorage
{
private:
    QList<TestCase> cases_;
public:
    void inject();
    void addTestCase(TestCase test_case);

    // BufferStorage interface
public:
    virtual void setBuffer(port::ByteBuffer* newBuffer) override;
};
