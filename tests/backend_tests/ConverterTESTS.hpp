#pragma once

#include "common.h"

#include "../../include/backend/Converter.hpp"

class ScannerTests : public DebugLogger
{
private :
    PortScanner scanner_;
public:
    void segvTests();
    void throwingTests();
};

class PortOperatorTest
{
private :
    PortOperator operator_;
public:
    void segvTests();
    void throwingTests();

};

class DataHandlerTests
{
private :
    DataHandler dataHandler_;
public:
    void segvTests();
    void throwingTests();
};

class DataPackageTests
{
private :
    //DataPackage data_package_;
public:
    void segvTests();
    void throwingTests();
};

class ConverterTests : public QObject
{
    Q_OBJECT

private:
    ScannerTests scanner_test_;
public:
    ConverterTests();
    ~ConverterTests();

private slots:
    void segvTests();
    void throwingTests();
    void booleanTests();
    void valueTests();
};
