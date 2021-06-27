#include <QtTest>

// add necessary includes here

#include "../../include/backend/Converter.hpp"

class ScannerTests : public QObject
{
    Q_OBJECT

public:
    ScannerTests();
    ~ScannerTests();

private slots:
    void ConverterSEGVTests();
    void test_case2();

};



ScannerTests::ScannerTests()
{

}

ScannerTests::~ScannerTests()
{

}

QTEST_APPLESS_MAIN(ScannerTests)

#include "PortScanner_TEST.moc"
