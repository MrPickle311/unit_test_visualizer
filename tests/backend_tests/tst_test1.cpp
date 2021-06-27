#include <QtTest>

// add necessary includes here

#include "../../include/backend/Converter.hpp"

///

class DebugLogger
{
public:
    template<typename DataType>
    static void sendListDataToDebug(QList<DataType>&& data);
};

template<typename DataType>
void DebugLogger::sendListDataToDebug(QList<DataType>&& data)
{
    for(auto&& line : data)
        qDebug() << line;
}

///

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


QTEST_APPLESS_MAIN(ConverterTests)

#include "tst_test1.moc"
