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



//class UnitTestInjecter:
//        public UnitTestDataPackage,
//        public BufferStorage
//{
//private:
//    void injectName();
//    void injectType();
//    void injectCurrentValue();
//    void injectTestResult();
//    void injectEnd();
//    void injectExpectedValue();
//    void injectLowerValue();
//    void injectUpperValue();
//protected:
//    void injectMultiValue(QList<uint8_t> values);
//    virtual void injectValues() = 0;
//public:
//    void inject();
//};
//
//class TestCaseInjecter:
//        public BufferStorage,
//        public TestCase
//{
//private:
//    void injectCaseName();
//public:
//    void inject();
//    virtual void setBuffer(port::ByteBuffer* newBuffer) override;
//};
//
//class TransactionInjecter:
//        public Transaction,
//        public BufferStorage
//{
//public:
//    virtual void setBuffer(port::ByteBuffer* newBuffer) override;
//    void inject();
//};
