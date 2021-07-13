#include "Parser_UnitTests.hpp"
#include <QObject>

void PackageTester::setPackage(const parser::DataPackage& newPackage)
{
    package_ = newPackage;
}

void PackageTester::expectPackageTypeDescriptor(parser::TypeDescriptor desc)
{
    EXPECT_EQ(package_.parsed_data_[parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR][0] , desc);//only first byte
}

void PackageTester::expectPackageName(std::string name)
{
    EXPECT_STREQ(package_.parsed_data_[parser::UnitTestCommand::SENDING_NAME].data() , name.c_str());//only first byte
}

void PackageTester::expectPackageValues(parser::UnitTestCommand cmd ,QList<uint8_t> values)
{
    for(int i{0}; i < values.size() ; ++i)
        EXPECT_EQ(static_cast<uint8_t>(package_.parsed_data_[cmd][i]) , values[i]);
}

void PackageTester::expectPackageCurrentValues(QList<uint8_t> values)
{
    expectPackageValues(parser::UnitTestCommand::SENDING_CURRENT_VALUE , values);
}

void PackageTester::expectPackageExpectedValues(QList<uint8_t> values)
{
    expectPackageValues(parser::UnitTestCommand::SENDING_EXPECTED_VALUE , values);
}

void PackageTester::expectPackageTestResult(TestResult result )
{
    EXPECT_EQ(package_.parsed_data_[parser::UnitTestCommand::SENDING_TEST_RESULT][0] , result);
}

///

void AbstractLocalParser_UnitTests::appendName(std::string name)
{
    name_ = name;
}

void AbstractLocalParser_UnitTests::appendType(parser::TypeDescriptor desc)
{
    descriptor_ = desc;
}

void AbstractLocalParser_UnitTests::appendExpectedValues(QList<uint8_t> values)
{
    expected_values_ = values;
}

void AbstractLocalParser_UnitTests::appendCurrentValues(QList<uint8_t> values)
{
    current_values_ = values;
}

void AbstractLocalParser_UnitTests::appendTestResult(TestResult result)
{
    result_ = result;
}

void AbstractLocalParser_UnitTests::appendEnd(){} //empty

///

LocalParser_UnitTests::LocalParser_UnitTests():
    buffer_{},
    local_parser_(&buffer_),
    test_body_{}
{}

void LocalParser_UnitTests::appendCode(uint8_t code)
{
    buffer_.appendByte(code);
}

void LocalParser_UnitTests::appendName(std::string name)
{
    test_body_.appendName(name);

    appendCode(parser::UnitTestCommand::SENDING_NAME);
    for(auto&& c : name)
        appendCode(c);
    appendCode('\0');
}

void LocalParser_UnitTests::appendType(parser::TypeDescriptor desc)
{
    test_body_.appendType(desc);

    appendCode(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR);
    appendCode(desc);
}

void LocalParser_UnitTests::appendTestResult(TestResult result)
{
    test_body_.appendTestResult(result);

    appendCode(parser::UnitTestCommand::SENDING_TEST_RESULT);
    appendCode(result);
}

void LocalParser_UnitTests::appendEnd()
{
    test_body_.appendEnd();

    appendCode(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT);
}

void LocalParser_UnitTests::appendExpectedValues(QList<uint8_t> values)
{
    test_body_.appendExpectedValues(values);

    appendCode(parser::UnitTestCommand::SENDING_EXPECTED_VALUE);
    for(auto&& value : values)
        appendCode(value);
}

void LocalParser_UnitTests::appendCurrentValues(QList<uint8_t> values)
{
    test_body_.appendCurrentValues(values);

    appendCode(parser::UnitTestCommand::SENDING_CURRENT_VALUE);
    for(auto&& value : values)
        appendCode(value);
}

void LocalParser_UnitTests::expectParserReady() const
{
    EXPECT_TRUE(local_parser_.packageReady());
}

void LocalParser_UnitTests::expectParserNotReady() const
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

TEST_F(LocalParser_UnitTests , BoolVariableTest)
{
    appendType(parser::TypeDescriptor::BOOL);
    appendName("xd()");
    appendExpectedValues({1});
    appendCurrentValues({1});
    appendTestResult(PASSED);
    appendEnd();

    local_parser_.parseData();

    expectParserReady();

    package_tester_.setPackage(local_parser_.getParsedPackage());

   // parser::DataPackage package {};//it will be interface , but i dont care now

    package_tester_.expectPackageTypeDescriptor(parser::TypeDescriptor::BOOL);
    package_tester_.expectPackageName("xd()");
    package_tester_.expectPackageExpectedValues({1});
    package_tester_.expectPackageCurrentValues({1});

    package_tester_.expectPackageTestResult(PASSED);

    //here parser is empty

    expectParserNotReady();

    expectEmptyResult();
}

TEST_F(LocalParser_UnitTests , Uint32VariableTest)
{
    appendType(parser::TypeDescriptor::UINT32_T);
    appendName("xc");
    appendExpectedValues({177 , 1 , 0 , 0});
    appendCurrentValues({177 , 1 , 0 , 0});
    appendTestResult(PASSED);
    appendEnd();

    local_parser_.parseData();

    expectParserReady();

    package_tester_.setPackage(local_parser_.getParsedPackage());

    package_tester_.expectPackageTypeDescriptor( parser::TypeDescriptor::UINT32_T);
    package_tester_.expectPackageName("xc");
    package_tester_.expectPackageExpectedValues( {177 , 1 , 0 , 0});
    package_tester_.expectPackageCurrentValues( {177 , 1 , 0 , 0});

    package_tester_.expectPackageTestResult(PASSED);

    //here parser is empty

    expectParserNotReady();

    expectEmptyResult();
}

TEST_F(LocalParser_UnitTests , Int64VariableTest)
{
    appendType(parser::TypeDescriptor::INT64_T);
    appendName("openmb");
    appendExpectedValues({177, 1, 0, 0, 0, 0, 0 ,0});
    appendCurrentValues({118, 194, 250, 255, 255, 255, 255, 255});
    appendTestResult(FAILURE);
    appendEnd();

    local_parser_.parseData();

    expectParserReady();

    package_tester_.setPackage(local_parser_.getParsedPackage());

    package_tester_.expectPackageTypeDescriptor(parser::TypeDescriptor::INT64_T);
    package_tester_.expectPackageName( "openmb");
    package_tester_.expectPackageExpectedValues( {177, 1, 0, 0, 0, 0, 0 ,0});
    package_tester_.expectPackageCurrentValues(  {118, 194, 250, 255, 255, 255, 255, 255});

    package_tester_.expectPackageTestResult( FAILURE);

    //here parser is empty

    expectParserNotReady();

    expectEmptyResult();
}


