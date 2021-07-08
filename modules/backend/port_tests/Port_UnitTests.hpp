#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../PortOperator.hpp"
#include "../PortSettings.hpp"
#include <QObject>
#include <string>
#include "common.hpp"

using namespace testing;

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

class DataHandlerTEST:
        public ::testing::Test
{
protected:
    port::DataHandler handler_;
public:
    DataHandlerTEST();
    void        appendChars(std::string bytes);
    QByteArray  emptyHandler();
    bool        isHandlerEmpty() const;
    size_t      currentBytesCount() const;
    QByteArray  getSeveralBytes(size_t count);
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

class PortInputOperatorTEST:
        public DataHandlerTEST
{
protected:
    port::PortScanner         scanner_;
    port::PortInputOperator   operator_;
public:
    PortInputOperatorTEST();
    void selectPort(uint port_nmbr);
    void showPorts() const;
    void waitAndShowArrivingData();
    void openPort();
};

