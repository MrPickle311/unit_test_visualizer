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


ByteBufferTEST_body::ByteBufferTEST_body()
    : buffer_{}
{}

void ByteBufferTEST_body::appendCharsToBuffer(std::string bytes)
{
    buffer_.appendBytes(QByteArray{bytes.c_str()});
}

QByteArray ByteBufferTEST_body::emptyBuffer()
{
    return buffer_.getAllBytes();
}

bool ByteBufferTEST_body::isBufferEmpty() const
{
    return buffer_.isEmpty();
}

size_t ByteBufferTEST_body::currentBytesCount() const
{
    return buffer_.size();
}

QByteArray ByteBufferTEST_body::getSeveralBytes(size_t count)
{
    return buffer_.getBytes(count);
}



TEST_F(ByteBufferTEST_body, ThrowingTest)
{
    EXPECT_NO_THROW ( appendCharsToBuffer("abcdef")) ;

    EXPECT_NO_THROW( emptyBuffer());
    EXPECT_NO_THROW(  emptyBuffer());
    EXPECT_THROW( getSeveralBytes(FIVE_BYTES), std::logic_error);

    appendCharsToBuffer("abcde");

    EXPECT_NO_THROW(getSeveralBytes(FIVE_BYTES));

    EXPECT_THROW( getSeveralBytes(ONE_BYTE), std::logic_error);

    EXPECT_NO_THROW(getSeveralBytes(NO_BYTES));
}



void ByteBufferTEST_body::expectCurrentBytesCountInBuffer(size_t bytes_count)
{
    EXPECT_EQ(currentBytesCount() , bytes_count );
}

void ByteBufferTEST_body::emptyTest(QByteArray& bytes , std::string expected_bytes)
{
    EXPECT_STREQ(bytes.data(),expected_bytes.c_str());
    expectCurrentBytesCountInBuffer(NO_BYTES);
}

void ByteBufferTEST_body::popTest(QByteArray& bytes , std::string expected_bytes,
                                  size_t expected_bytes_count_in_buffer)
{
    bytes = getSeveralBytes(expected_bytes.size());
    EXPECT_STREQ(bytes.data(),expected_bytes.c_str());
    expectCurrentBytesCountInBuffer(expected_bytes_count_in_buffer);
}

TEST_F(ByteBufferTEST_body, LogicTest)
{
    QByteArray bytes;
    appendCharsToBuffer("abcde");

    EXPECT_FALSE(isBufferEmpty());

    bytes = emptyBuffer();
    emptyTest(bytes,"abcde");

    EXPECT_TRUE(isBufferEmpty());

    appendCharsToBuffer("112kk");
    expectCurrentBytesCountInBuffer(FIVE_BYTES);

    emptyBuffer();

    appendCharsToBuffer("");
    expectCurrentBytesCountInBuffer(NO_BYTES);

    appendCharsToBuffer("abcde");

    popTest(bytes, "ab" , THREE_BYTES);

    popTest(bytes, "c" , TWO_BYTES);
}

void ByteBuffer_SignalTester::expectBytes(size_t count)
{
    EXPECT_EQ(count, FIVE_BYTES);
}

void ByteBuffer_SignalTester::expectEmptyHandler(size_t count)
{
    EXPECT_EQ(count , FIVE_BYTES );
}

ByteBuffer_SignalTester::~ByteBuffer_SignalTester(){}

TEST_F(ByteBufferTEST_body, SignalTest)
{
   ByteBuffer_SignalTester tester;

   QObject::connect(&buffer_ , &port::ByteBuffer::bytesArrived , &tester ,
                     &ByteBuffer_SignalTester::expectBytes);
   QObject::connect(&buffer_ , &port::ByteBuffer::bytesExtracted , &tester ,
                     &ByteBuffer_SignalTester::expectEmptyHandler);

   appendCharsToBuffer("abcde");
   emptyBuffer();
   EXPECT_TRUE(isBufferEmpty());
}




