#include "DataHandler_TESTS.hpp"
#include <gtest/gtest.h>
#include <QObject>
#include <QBuffer>

DataHandlerTEST::DataHandlerTEST()
    : handler_{}
{}

void DataHandlerTEST::appendChars(std::string bytes)
{
    handler_.appendReceivedBytes(QByteArray{bytes.c_str()});
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

#define NO_BYTES     0
#define ONE_BYTE     1
#define TWO_BYTES    2
#define THREE_BYTES  3
#define FIVE_BYTES   5

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

    appendChars("abcde");
    bytes = getSeveralBytes(TWO_BYTES);

    EXPECT_STREQ(bytes.data(),"ab");
    EXPECT_EQ(handler_.size(),THREE_BYTES);

    bytes = getSeveralBytes(ONE_BYTE);

    EXPECT_STREQ(bytes.data(),"c");
    EXPECT_EQ(handler_.size(),TWO_BYTES);
}

void DataHandler_SignalTester::expectBytes(size_t count)
{
    EXPECT_EQ(count, FIVE_BYTES);
}

void DataHandler_SignalTester::expectEmptyHandler(size_t count)
{
    EXPECT_EQ(count , FIVE_BYTES );
}

DataHandler_SignalTester::~DataHandler_SignalTester(){}

TEST_F(DataHandlerTEST, SignalTest)
{
   DataHandler_SignalTester tester;

   QObject::connect(&handler_ , &DataHandler::bytesArrived , &tester ,
                     &DataHandler_SignalTester::expectBytes);
   QObject::connect(&handler_ , &DataHandler::bytesExtracted , &tester ,
                     &DataHandler_SignalTester::expectEmptyHandler);

   appendChars("abcde");
   emptyHandler();
   EXPECT_TRUE(isHandlerEmpty());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


