#include "DataHandler_TESTS.hpp"
#include <gtest/gtest.h>

DataHandlerTEST::DataHandlerTEST()
    : handler_{}
{}

void DataHandlerTEST::appendChars(std::string bytes)
{
    handler_.appendReceivedBytes(QByteArray{bytes.c_str()});
}

void DataHandlerTEST::showCurrentData() const
{

}

QByteArray DataHandlerTEST::emptyHandler()
{
    return handler_.getAllReceivedBytes();
}

bool DataHandlerTEST::isHandlerEmpty() const
{
    return handler_.isEmpty();
}

size_t DataHandlerTEST::currentBytesCount() const
{
    return handler_.size();
}

QByteArray DataHandlerTEST::getSeveralBytes(size_t count)
{
    return handler_.getReceivedBytes(count);
}


TEST_F(DataHandlerTEST, MovingTest)
{



}

#define FIVE_BYTES 5
#define ONE_BYTE   1
#define NO_BYTES   0

TEST_F(DataHandlerTEST, ThrowingTest)
{
    EXPECT_NO_THROW ( appendChars("abcdef")) ;

    EXPECT_NO_THROW( emptyHandler());
    EXPECT_NO_THROW(  emptyHandler());
    EXPECT_THROW( getSeveralBytes(FIVE_BYTES), std::logic_error);

    appendChars("abcde");

    EXPECT_NO_THROW(getSeveralBytes(FIVE_BYTES));

    EXPECT_THROW( getSeveralBytes(ONE_BYTE), std::logic_error);

    EXPECT_NO_THROW(getSeveralBytes(NO_BYTES));
}

TEST_F(DataHandlerTEST, LogicTest)
{
    QByteArray bytes;
    appendChars("abcde");

    EXPECT_FALSE(isHandlerEmpty());

    bytes = emptyHandler();

    EXPECT_STREQ(bytes.data(),"abcde");
    EXPECT_TRUE(isHandlerEmpty());
    EXPECT_EQ(currentBytesCount() , NO_BYTES );

    appendChars("112kk");
    EXPECT_EQ(currentBytesCount() , FIVE_BYTES );

    emptyHandler();

    appendChars("");
    EXPECT_EQ(currentBytesCount() , NO_BYTES );
}

TEST_F(DataHandlerTEST, SignalTest)
{

}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


