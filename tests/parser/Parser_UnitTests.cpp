#include "Parser_UnitTests.hpp"
#include <QObject>

LocalParser_UnitTests::LocalParser_UnitTests():
    buffer_{},
    local_parser_(&buffer_)
{}

void LocalParser_UnitTests::appendCode(uint8_t code)
{
    buffer_.appendByte(code);
}

void LocalParser_UnitTests::appendName(std::string name)
{
    appendCode(parser::UnitTestCommand::SENDING_NAME);
    for(auto&& c : name)
        appendCode(c);
    appendCode('\0');
}

void LocalParser_UnitTests::appendType(parser::TypeDescriptor desc)
{
    appendCode(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR);
    appendCode(desc);
}

void LocalParser_UnitTests::appendTestResult(TestResult result)
{
    appendCode(parser::UnitTestCommand::SENDING_TEST_RESULT);
    appendCode(result);
}

void LocalParser_UnitTests::appendEnd()
{
    appendCode(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT);
}

void LocalParser_UnitTests::expectPackageReady() const
{
    EXPECT_TRUE(local_parser_.packageReady());
}

void LocalParser_UnitTests::expectPackageNotReady() const
{
    EXPECT_FALSE(local_parser_.packageReady());
}

void LocalParser_UnitTests::expectEmptyResult() const
{
    EXPECT_TRUE(local_parser_.isEmptyResult());
}

void LocalParser_UnitTests::expectNotEmptyResult() const
{
    EXPECT_FALSE(local_parser_.isEmptyResult());
}

TEST_F(LocalParser_UnitTests , DependencyTest)
{
    EXPECT_NE(local_parser_.getBuffer() , nullptr);
}

void expectPackageTypeDescriptor(const parser::DataPackage& package, parser::TypeDescriptor desc)
{
    EXPECT_EQ(package.parsed_data_[parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR][0] , desc);//only first byte
}

void expectPackageName(const parser::DataPackage& package, std::string name)
{
    EXPECT_STREQ(package.parsed_data_[parser::UnitTestCommand::SENDING_NAME].data() , name.c_str());//only first byte
}

void expectPackageValues(const parser::DataPackage& package, parser::UnitTestCommand cmd ,QList<uint8_t> values)
{
    for(int i{0}; i < values.size() ; ++i)
        EXPECT_EQ(package.parsed_data_[cmd][i] , values[i]);
}

void expectPackageCurrentValues(const parser::DataPackage& package, QList<uint8_t> values)
{
    expectPackageValues(package, parser::UnitTestCommand::SENDING_CURRENT_VALUE , values);
}

void expectPackageExpectedValues(const parser::DataPackage& package, QList<uint8_t> values)
{
    expectPackageValues(package, parser::UnitTestCommand::SENDING_EXPECTED_VALUE , values);
}

void expectPackageTestResult(const parser::DataPackage& package , TestResult result )
{
    EXPECT_EQ(package.parsed_data_[parser::GlobalCommand::SENDING_UNIT_TEST_RESULT][0] , result);
}

TEST_F(LocalParser_UnitTests , BoolVariableTest)
{
    appendType(parser::TypeDescriptor::BOOL);
    appendName("xd()");
    appendExpectedValues(1);
    appendCurrentValues(1);
    appendTestResult(PASSED);
    appendEnd();

    local_parser_.parseData();

    expectPackageReady();

    parser::DataPackage package {local_parser_.getParsedPackage()};//it will be interface , but i dont care now

    expectPackageTypeDescriptor(package , parser::TypeDescriptor::BOOL);
    expectPackageName(package, "xd()");
    expectPackageExpectedValues(package , {1});
    expectPackageCurrentValues(package , {1});

    expectPackageTestResult(package , PASSED);

    //here parser is empty

    expectPackageNotReady();

    expectEmptyResult();
}

TEST_F(LocalParser_UnitTests , Uint32VariableTest)
{
    appendType(parser::TypeDescriptor::UINT32_T);
    appendName("xc");
    appendExpectedValues(177 , 1 , 0 , 0);
    appendCurrentValues(177 , 1 , 0 , 0);
    appendTestResult(PASSED);
    appendEnd();

    local_parser_.parseData();

    expectPackageReady();

    parser::DataPackage package {local_parser_.getParsedPackage()};//it will be interface , but i dont care now

    expectPackageTypeDescriptor(package , parser::TypeDescriptor::UINT32_T);
    expectPackageName(package, "xc");
    expectPackageExpectedValues(package , {177 , 1 , 0 , 0});
    expectPackageCurrentValues(package , {177 , 1 , 0 , 0});

    expectPackageTestResult(package , PASSED);

    //here parser is empty

    expectPackageNotReady();

    expectEmptyResult();
}
