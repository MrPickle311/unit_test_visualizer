#include "Parser_UnitTests.hpp"
#include <QObject>

LocalParser_UnitTests::LocalParser_UnitTests()
{
    local_parser_.setBuffer(&buffer_);//drops data from buffer

    //this moves to upper function
    QObject::connect(&buffer_ , &port::ByteBuffer::bytesArrived ,
                     &local_parser_ ,&LocalByteParser::parseByte );

}

void LocalParser_UnitTests::appendCode(uint8_t code)
{
    buffer_.appendByte(code);
}

TEST_F(LocalParser_UnitTests , DependencyTest)
{
    EXPECT_NE(local_parser_.getBuffer() , nullptr);
}

TEST_F(LocalParser_UnitTests , LogicTest)
{
    appendCode(TestCaseCommand::SENDING_NAME);

    appendCode(120);
    appendCode(100);
    appendCode(40);
    appendCode(41);

    appendCode(TestCaseCommand::SENDING_TYPE_DESCRIPTOR);
    appendCode(8);

    appendCode(TestCaseCommand::SENDING_EXPECTED_VALUE);
    appendCode(1);

    appendCode(TestCaseCommand::SENDING_CURRENT_VALUE);
    appendCode(1);

    appendCode(TestCaseCommand::END_SENDING_UNIT_TEST_RESULT);

    UnitTestPackage package {local_parser_.getParsedPackage()};//it will be interface , but i dont care now

    EXPECT_TRUE(local_parser_.packageReady());

    EXPECT_STREQ(package.unit_test_name_.data() , "xd()");
    EXPECT_EQ(package.type_descriptor_ , 8);
    EXPECT_EQ(package.parsed_data_[0][0] , 1);
    EXPECT_EQ(package.parsed_data_[1][0] , 1);
    EXPECT_EQ(package.parsed_data_[2][0] , 1);

    //here parser is empty

    EXPECT_FALSE(local_parser_.packageReady());

    EXPECT_TRUE(local_parser_.atStart());//useless maybe
}
