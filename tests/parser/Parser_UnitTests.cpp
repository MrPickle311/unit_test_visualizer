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

TEST_F(LocalParser_UnitTests , DependencyTest)
{
    EXPECT_NE(local_parser_.getBuffer() , nullptr);
}

TEST_F(LocalParser_UnitTests , BoolVariableTest)
{
    appendCode(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR);
    appendCode(parser::TypeDescriptor::BOOL);

    appendCode(parser::UnitTestCommand::SENDING_NAME);

    appendCode('x');
    appendCode('d');
    appendCode('(');
    appendCode(')');
    appendCode('\0');

    appendCode(parser::UnitTestCommand::SENDING_EXPECTED_VALUE);
    appendCode(1);

    appendCode(parser::UnitTestCommand::SENDING_CURRENT_VALUE);
    appendCode(1);

    appendCode(parser::UnitTestCommand::SENDING_TEST_RESULT);
    appendCode(1);

    appendCode(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT);

    local_parser_.parseData();

    parser::DataPackage package {local_parser_.getParsedPackage()};//it will be interface , but i dont care now

    EXPECT_TRUE(local_parser_.packageReady());

    EXPECT_EQ(package.parsed_data_[0][0] , 8);
    EXPECT_STREQ(package.parsed_data_[1].data() , "xd()");
    EXPECT_EQ(package.parsed_data_[2][0] , 1);
    EXPECT_EQ(package.parsed_data_[3][0] , 1);
    EXPECT_EQ(package.parsed_data_[4][0] , 1);

    //here parser is empty

    EXPECT_FALSE(local_parser_.packageReady());

    EXPECT_TRUE(local_parser_.isEmptyResult());//useless maybe
}
