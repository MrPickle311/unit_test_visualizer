#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../PortOperator.hpp"
#include "../PortSettings.hpp"
#include <QObject>
#include <string>
#include "common.hpp"

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

class DataHandlerTEST_body:
        public ::testing::Test
{
protected:
    port::DataHandler handler_;
public:
    DataHandlerTEST_body();
    void        appendChars(std::string bytes);
    QByteArray  emptyHandler();
    bool        isHandlerEmpty() const;
    size_t      currentBytesCount() const;
    QByteArray  getSeveralBytes(size_t count);
};

class XD: public DataHandlerTEST_body
{

};

class DataHandler_SignalTester:
        public QObject
{
    Q_OBJECT;
public slots:
    void expectBytes(size_t count);
    void expectEmptyHandler(size_t count);
public:
    ~DataHandler_SignalTester();
};



