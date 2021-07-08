#include "Port_UnitTests.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

void ScannerTests::segvTests()
{
    sendListDataToDebug(scanner_.getPortNames());
    sendListDataToDebug(scanner_.getPortDescriptions());
    sendListDataToDebug(scanner_.getProductIndetifiers());
}

void ScannerTests::throwingTests()
{
    EXPECT_THROW(scanner_.getSelectedPort(50), std::logic_error );
}


DataHandlerTEST_body::DataHandlerTEST_body()
    : handler_{}
{}

void DataHandlerTEST_body::appendChars(std::string bytes)
{
    handler_.appendReceivedBytes(QByteArray{bytes.c_str()});
}

QByteArray DataHandlerTEST_body::emptyHandler()
{
    return handler_.getAllReceivedBytes();
}

bool DataHandlerTEST_body::isHandlerEmpty() const
{
    return handler_.isEmpty();
}

size_t DataHandlerTEST_body::currentBytesCount() const
{
    return handler_.size();
}

QByteArray DataHandlerTEST_body::getSeveralBytes(size_t count)
{
    return handler_.getReceivedBytes(count);
}



TEST_F(DataHandlerTEST_body, ThrowingTest)
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

//TO REFACTOR
TEST_F(DataHandlerTEST_body, LogicTest)
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

TEST_F(DataHandlerTEST_body, SignalTest)
{
   DataHandler_SignalTester tester;

   QObject::connect(&handler_ , &port::DataHandler::bytesArrived , &tester ,
                     &DataHandler_SignalTester::expectBytes);
   QObject::connect(&handler_ , &port::DataHandler::bytesExtracted , &tester ,
                     &DataHandler_SignalTester::expectEmptyHandler);

   appendChars("abcde");
   emptyHandler();
   EXPECT_TRUE(isHandlerEmpty());
}




