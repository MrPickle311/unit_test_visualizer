#pragma once

#include <PortOperator.hpp>
#include <PortSettings.hpp>
#include <common.hpp>

class ScannerTests :
        public DebugLogger,
        public ::testing::Test
{
private :
    port::PortScanner scanner_;
public:
    void segvTests();
    void throwingTests();
};

class ByteBufferTEST_body:
        public ::testing::Test
{
protected:
    port::ByteBuffer buffer_;
public:
    ByteBufferTEST_body();
    void        appendCharsToBuffer(std::string bytes);
    QByteArray  emptyBuffer();
    bool        isBufferEmpty() const;
    size_t      currentBytesCount() const;
    QByteArray  getSeveralBytes(size_t count);
    void        emptyTest(QByteArray& bytes, std::string expected_bytes);
    void expectCurrentBytesCountInBuffer(size_t bytes_count);
    void popTest(QByteArray& bytes, std::string expected_bytes,
                 size_t expected_bytes_count_in_buffer);
};

class ByteBuffer_SignalTester:
        public QObject
{
    Q_OBJECT;
public slots:
    void expectBytes(size_t count);
    void expectEmptyHandler(size_t count);
public:
    ~ByteBuffer_SignalTester();
};



