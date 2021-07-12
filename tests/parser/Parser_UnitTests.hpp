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

class LocalParser_UnitTests:
        public ::testing::Test
{
protected:
    port::ByteBuffer buffer_;
    parser::UnitTestLocalByteParser local_parser_;
public:
    LocalParser_UnitTests();
    void appendCode(uint8_t code);
    void appendName(std::string name);
    void appendType(parser::TypeDescriptor desc);
    void appendTestResult(TestResult);
    void appendEnd();
    void expectPackageReady() const;
    void expectEmptyResult() const;
    void expectNotEmptyResult() const;
    void expectPackageNotReady() const;


    template<typename... Args>
    void appendExpectedValues(Args... args)
    {
        appendCode(parser::UnitTestCommand::SENDING_EXPECTED_VALUE);
        (appendCode(args), ...);
    }
    template<typename... Args>
    void appendCurrentValues(Args... args)
    {
        appendCode(parser::UnitTestCommand::SENDING_CURRENT_VALUE);
        (appendCode(args), ...);
    }
};
