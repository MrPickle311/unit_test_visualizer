#pragma once

#include <common.hpp>
#include <Parser.hpp>
#include <PortOperator.hpp>

//parser erases all commands and splits incoming data into
//certain data
//converter converts each package to full text which can be
//send to frontend-qml

using TestResult = int;

#define FAILURE 0
#define PASSED  1

class PackageTester
{
public:
    void expectPackageTypeDescriptor(parser::TypeDescriptor desc);
    void setPackage(const parser::DataPackage& newPackage);

    void expectPackageTestResult(TestResult result);
    void expectPackageExpectedValues(QList<uint8_t> values);
    void expectPackageCurrentValues(QList<uint8_t> values);
    void expectPackageName(std::string name);
private:
    parser::DataPackage package_;
    void expectPackageValues(parser::UnitTestCommand cmd, QList<uint8_t> values);
};

class LocalParser_UnitTests_interface
{
public:
    virtual void appendName(std::string name) = 0;
    virtual void appendType(parser::TypeDescriptor desc) = 0;
    virtual void appendExpectedValues(QList<uint8_t> values) = 0;
    virtual void appendCurrentValues(QList<uint8_t> values) = 0;
    virtual void appendTestResult(TestResult) = 0;
    virtual void appendEnd() = 0;
};

class AbstractLocalParser_UnitTests:
    public LocalParser_UnitTests_interface
{
protected:
    QList<uint8_t> expected_values_;
    QList<uint8_t> current_values_;
    std::string name_;
    parser::TypeDescriptor descriptor_;
    TestResult  result_;

    // LocalParser_UnitTests_interface interface
public:
    virtual void appendName(std::string name) override;
    virtual void appendType(parser::TypeDescriptor desc) override;
    virtual void appendExpectedValues(QList<uint8_t> values) override;
    virtual void appendCurrentValues(QList<uint8_t> values) override;
    virtual void appendTestResult(TestResult) override;
    virtual void appendEnd() override;
};

class LocalParser_UnitTests:
        public ::testing::Test,
        public LocalParser_UnitTests_interface
{
protected:
    port::ByteBuffer                buffer_;
    parser::UnitTestLocalByteParser local_parser_;
    AbstractLocalParser_UnitTests   test_body_;
    PackageTester                   package_tester_;
public:
    LocalParser_UnitTests();
public:
    void appendCode(uint8_t code);
    void expectParserReady() const;
    void expectEmptyResult() const;
    void expectNotEmptyResult() const;
    void expectParserNotReady() const;
public:
    virtual void appendName(std::string name) override;
    virtual void appendType(parser::TypeDescriptor desc) override;
    virtual void appendTestResult(TestResult) override;
    virtual void appendEnd() override;
    virtual void appendExpectedValues(QList<uint8_t> values) override;
    virtual void appendCurrentValues(QList<uint8_t> values) override;
public:
    void run();
};


