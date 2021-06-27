#include <QtTest>

#include "../../main.h"
// add necessary includes here

class test1 : public QObject
{
    Q_OBJECT

public:
    Receiver rec_;
    test1();
    ~test1();

private slots:
    void test_case1();

};

test1::test1():
    rec_{}
{

}

test1::~test1()
{

}

void test1::test_case1()
{
    QCOMPARE(rec_.objectName(), "xd");
}

QTEST_APPLESS_MAIN(test1)

#include "tst_test1.moc"
