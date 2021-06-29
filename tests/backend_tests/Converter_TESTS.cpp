#include "common.h"
#include "ConverterTESTS.hpp"

// add necessary includes here

#include "../../include/backend/Converter.hpp"


class ScannerTests : public DebugLogger
{
private :
    PortScanner scanner_;
public:
    void segvTests();
    void throwingTests();
};

void ScannerTests::segvTests()
{
    sendListDataToDebug(scanner_.getPortNames());
    sendListDataToDebug(scanner_.getPortDescriptions());
    sendListDataToDebug(scanner_.getProductIndetifiers());
}

void ScannerTests::throwingTests()
{
    QVERIFY_EXCEPTION_THROWN(scanner_.getSelectedPort(50), std::logic_error );
}

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

ConverterTests::ConverterTests():
    scanner_test_{}
{}

ConverterTests::~ConverterTests()
{}


void ConverterTests::segvTests()
{
    scanner_test_.segvTests();
}

void ConverterTests::throwingTests()
{
    scanner_test_.throwingTests();
}

void ConverterTests::booleanTests()
{

}

void ConverterTests::valueTests()
{

}


QTEST_APPLESS_MAIN(ConverterTests)

#include "tst_test1.moc"
