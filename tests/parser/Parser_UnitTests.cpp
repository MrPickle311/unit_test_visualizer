#include "Parser_UnitTests.hpp"
#include <QObject>

template<typename T>
using Ptr = QSharedPointer<T>;

QSharedPointer<backend::UnitTestDataPackage> TestPackageFactory::allocPackage()
{
    return QSharedPointer<backend::UnitTestDataPackage>::create();
}

ParserTests::ParserTests():
    result_{},
    root_{backend::ReadyParsers::getValue(backend::ParserImplementations::FirstImplementation)},
    buffer_{Ptr<backend::ByteBuffer>::create()}
{
    root_->setBuffer(buffer_.data());
}

QSharedPointer<backend::UnitTestDataPackage> TestPackageFactory::createBoolUnitTest()
{
    QSharedPointer<backend::UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({backend::TypeDescriptor::BOOL}));
    pack->setName("xd()");
    pack->setExpectedValue(bytes({1}));
    pack->setCurrentValue(bytes({1}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<backend::UnitTestDataPackage> TestPackageFactory::createUint32UnitTest()
{
    QSharedPointer<backend::UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({backend::TypeDescriptor::UINT32_T}));
    pack->setName("xc");
    pack->setExpectedValue(bytes({177 , 1 , 0 , 0}));
    pack->setCurrentValue(bytes({177 , 1 , 0 , 0}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<backend::UnitTestDataPackage> TestPackageFactory::createBitUnitTest()
{
    QSharedPointer<backend::UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({backend::TypeDescriptor::BIT}));
    pack->setName("reg");
    pack->setExpectedValue(bytes({1}));
    pack->setCurrentValue(bytes({1}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<backend::UnitTestDataPackage> TestPackageFactory::createInt64UnitTest()
{
    QSharedPointer<backend::UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({backend::TypeDescriptor::INT64_T}));
    pack->setName("openmb");
    pack->setExpectedValue(bytes({177, 1, 0, 0, 0, 0, 0 ,0}));
    pack->setCurrentValue(bytes({118, 194, 250, 255, 255, 255, 255, 255}));
    pack->setResult(bytes({FAILURE}));
    return pack;

}

QSharedPointer<backend::UnitTestDataPackage> TestPackageFactory::createUint64UnitTest()
{
    QSharedPointer<backend::UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({backend::TypeDescriptor::UINT64_T}));
    pack->setName("var1");
    pack->setExpectedValue(bytes({177, 1, 0, 0, 0, 0, 0 ,0}));
    pack->setCurrentValue(bytes({118, 194, 250, 255, 255, 255, 255, 255}));
    pack->setResult(bytes({FAILURE}));
    return pack;
}

QSharedPointer<backend::UnitTestDataPackage> TestPackageFactory::createPtrUnitTest()
{
    QSharedPointer<backend::UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({backend::TypeDescriptor::PTR}));
    pack->setName("ptr");
    pack->setExpectedValue(bytes({0}));
    pack->setCurrentValue(bytes({0}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<backend::UnitTestDataPackage> TestPackageFactory::createInt16RangeUnitTest()
{
    QSharedPointer<backend::UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({backend::TypeDescriptor::INT16_T}));
    pack->setName("dfname");
    pack->setCurrentValue(bytes({67 , 0}));
    pack->setLowerValue(bytes({249 , 255}));
    pack->setUpperValue(bytes({226 , 19}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

void ParserTests::insertDataAndRun(QSharedPointer<backend::TransactionDataPackage> transaction)
{
    injecter_.setBuffer(buffer_.data());
    injecter_.inject(transaction);

    root_->startProcessing();

    result_ = root_->getPackage();
}

TEST_F(ParserTests , SimpleLogicTest)
{
    auto test_case1{Ptr<backend::TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(factory_.createUint32UnitTest());

    auto transaction{Ptr<backend::TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    insertDataAndRun(transaction);

    EXPECT_TRUE(*result_ == *transaction);
}

TEST_F(ParserTests , ComplexLogicTest)
{
    auto test_case1{Ptr<backend::TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(factory_.createUint32UnitTest());
    test_case1->addUnitTest(factory_.createInt64UnitTest());
    test_case1->addUnitTest(factory_.createInt64UnitTest());
    test_case1->addUnitTest(factory_.createBitUnitTest());
    test_case1->addUnitTest(factory_.createPtrUnitTest());
    test_case1->addUnitTest(factory_.createInt16RangeUnitTest());

    auto transaction{Ptr<backend::TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    auto test_case2{Ptr<backend::TestCaseDataPackage>::create()};
    test_case2->setTestCaseName("test2");
    test_case2->addUnitTest(factory_.createBitUnitTest());
    test_case2->addUnitTest(factory_.createPtrUnitTest());

    transaction->addTestCase(test_case2);

    insertDataAndRun(transaction);

    EXPECT_TRUE(*result_ == *transaction);
}

TEST_F(ParserTests , ChildrenCountTest )
{
    auto test_case1{Ptr<backend::TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(factory_.createUint32UnitTest());
    test_case1->addUnitTest(factory_.createInt64UnitTest());
    test_case1->addUnitTest(factory_.createInt64UnitTest());
    test_case1->addUnitTest(factory_.createBitUnitTest());
    test_case1->addUnitTest(factory_.createPtrUnitTest());
    test_case1->addUnitTest(factory_.createInt16RangeUnitTest());

    auto transaction{Ptr<backend::TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    auto test_case2{Ptr<backend::TestCaseDataPackage>::create()};
    test_case2->setTestCaseName("test2");
    test_case2->addUnitTest(factory_.createBitUnitTest());
    test_case2->addUnitTest(factory_.createPtrUnitTest());

    transaction->addTestCase(test_case2);

    insertDataAndRun(transaction);

    EXPECT_STREQ(result_->getCases().at(0)->getTestCaseName() , "test1");
    EXPECT_STREQ(result_->getCases().at(1)->getTestCaseName() , "test2");

    EXPECT_EQ(result_->getCases().size() , 2);

    EXPECT_EQ(result_->getCases().at(0)->getTests().size() , 6);
    EXPECT_NE(result_->getCases().at(0)->getTests().size() , 5);
    EXPECT_EQ(result_->getCases().at(1)->getTests().size() , 2);
    EXPECT_NE(result_->getCases().at(1)->getTests().size() , 0);
}

TEST_F(ParserTests , EmptyTest )
{
    Ptr<backend::TransactionDataPackage> transaction{Ptr<backend::TransactionDataPackage>::create()};

    EXPECT_NO_FATAL_FAILURE(insertDataAndRun(transaction));
}

TEST_F(ParserTests , EmptyCase )
{
    auto test_case1{Ptr<backend::TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    auto transaction{Ptr<backend::TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    EXPECT_NO_FATAL_FAILURE(insertDataAndRun(transaction));
}

QByteArray bytes(QList<uint8_t> byte_lits)
{
    QByteArray result;
    for(auto&& byte : byte_lits)
        result.append(byte);
    return  result;
}
