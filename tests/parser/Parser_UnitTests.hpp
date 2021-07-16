#pragma once

#include <common.hpp>
#include <PortOperator.hpp>
#include "UnitTestBuilder.hpp"

//parser erases all commands and splits incoming data into
//certain data
//converter converts each package to full text which can be
//send to frontend-qml

//using TestResult = int;

#define FAILURE 0
#define PASSED  1

//class PackageTester
//{
//public:
//    void setPackage(const parser::DataPackage& newPackage);
//    void expectTypeDescriptor(parser::TypeDescriptor desc);
//    void expectTestResult(TestResult result);
//    void expectExpectedValue(QList<uint8_t> values);
//    void expectCurrentValue(QList<uint8_t> values);
//    void expectLowerValue(QList<uint8_t> values);
//    void expectUpperValue(QList<uint8_t> values);
//    void expectName(std::string name);
//private:
//    ParsedDataPackage package_;
//    void expectValue(parser::UnitTestCommand cmd, QList<uint8_t> values);
//};

//class LocalParser_UnitTests_interface
//{
//public:
//    virtual void appendName(std::string name) = 0;
//    virtual void appendType(parser::TypeDescriptor desc) = 0;
//    virtual void appendExpectedValue(QList<uint8_t> value) = 0;
//    virtual void appendCurrentValue(QList<uint8_t> value) = 0;
//    virtual void appendTestResult(TestResult) = 0;
//    virtual void appendLowerValue(QList<uint8_t> value) = 0;
//    virtual void appendUpperValue(QList<uint8_t> value) = 0;
//    virtual void appendEnd() = 0;
//};
//

//class UnitTestPackage
//{
//
//};
//
//class AbstractLocalParser_UnitTests:
//    public LocalParser_UnitTests_interface
//{
//    friend class LocalParser_UnitTests;
//protected:
//    QList<uint8_t> expected_value_;
//    QList<uint8_t> current_value_;
//    QList<uint8_t> lower_value_;
//    QList<uint8_t> upper_value_;
//    std::string name_;
//    parser::TypeDescriptor descriptor_;
//    TestResult  result_;
//
//    // LocalParser_UnitTests_interface interface
//public:
//    virtual void appendName(std::string name) override;
//    virtual void appendType(parser::TypeDescriptor desc) override;
//    virtual void appendExpectedValue(QList<uint8_t> values) override;
//    virtual void appendCurrentValue(QList<uint8_t> values) override;
//    virtual void appendTestResult(TestResult) override;
//    virtual void appendLowerValue(QList<uint8_t> value) override;
//    virtual void appendUpperValue(QList<uint8_t> value) override;
//    virtual void appendEnd() override;
//};
//

class BufferAppender
{
protected:
    port::ByteBuffer buffer_;
public:
    BufferAppender();//appends start
    void appendTestCase(TestCaseDataPackage test_case);
    void appendEnd();
};

//class LocalParser_UnitTests:
//        public ::testing::Test//,
//        //public LocalParser_UnitTests_interface
//{
//protected:
//    port::ByteBuffer                buffer_;
//    //parser::UnitTestLocalByteParser local_parser_;
//    //AbstractLocalParser_UnitTests   test_body_;
//    //PackageTester                   package_tester_;
//public:
//    LocalParser_UnitTests();
//public:
//    void appendCode(uint8_t code);
//    void expectParserReady() const;
//    void expectEmptyResult() const;
//    void expectNotEmptyResult() const;
//    void expectParserNotReady() const;
//private:
//    void appendSeveralCodes(QList<uint8_t> codes);
//    void fillPackage();
//    void checkCommon();
//public:
//    virtual void appendName(std::string name) override;
//    virtual void appendType(parser::TypeDescriptor desc) override;
//    virtual void appendExpectedValue(QList<uint8_t> values) override;
//    virtual void appendCurrentValue(QList<uint8_t> values) override;
//    virtual void appendTestResult(TestResult) override;
//    virtual void appendLowerValue(QList<uint8_t> value) override;
//    virtual void appendUpperValue(QList<uint8_t> value) override;
//    virtual void appendEnd() override;
//public:
//    void checkEqual();
//    void checkInRange();
//
//};


