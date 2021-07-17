#include "Parser_UnitTests.hpp"
#include <QObject>

template<typename T>
using Ptr = QSharedPointer<T>;

QByteArray bytes(QList<uint8_t> byte_lits)
{
    QByteArray result;
    for(auto&& byte : byte_lits)
        result.append(byte);
    return  result;
}

QSharedPointer<UnitTestDataPackage> TestPackageFactory::allocPackage()
{
    return QSharedPointer<UnitTestDataPackage>::create();
}

ParserTests::ParserTests():
    result_{},
    root_{Ptr<parser::GlobalParser>::create()},
    buffer_{Ptr<port::ByteBuffer>::create()}
{
    root_->setBuffer(buffer_.data());

    Ptr<parser::EndParser> end {Ptr<parser::EndParser>::create()};

    Ptr<parser::TestCaseParser> case_parser {Ptr<parser::TestCaseParser>::create()};

    root_->addChild(parser::GlobalCommand::START ,                 Ptr<parser::GlobalStartParser>::create());
    root_->addChild(parser::GlobalCommand::SENDING_TEST_CASE,      case_parser);
    root_->addChild(parser::GlobalCommand::END_ENTIRE_TRANSACTION, end);

    Ptr<parser::UnitTestParser> unit_parser{Ptr<parser::UnitTestParser>::create()};

    case_parser->addChild(parser::TestCaseCommand::SENDING_UNIT_TEST_RESULT , unit_parser);
    case_parser->addChild(parser::TestCaseCommand::END_SENDING_TEST_CASE ,    end);

    unit_parser->addChild(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR ,      Ptr<parser::TypeDescriptorParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_NAME ,                 Ptr<parser::NameParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_CURRENT_VALUE ,        Ptr<parser::CurrentValueParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_EXPECTED_VALUE ,       Ptr<parser::ExpectedValueParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_TEST_RESULT ,          Ptr<parser::TestResultParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_UPPER_VALUE ,          Ptr<parser::UpperValueParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_LOWER_VALUE ,          Ptr<parser::LowerValueParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT , end);
}

QSharedPointer<UnitTestDataPackage> TestPackageFactory::createBoolUnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::BOOL}));
    pack->setName("xd()");
    pack->setExpectedValue(bytes({1}));
    pack->setCurrentValue(bytes({1}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<UnitTestDataPackage> TestPackageFactory::createUint32UnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::UINT32_T}));
    pack->setName("xc");
    pack->setExpectedValue(bytes({177 , 1 , 0 , 0}));
    pack->setCurrentValue(bytes({177 , 1 , 0 , 0}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<UnitTestDataPackage> TestPackageFactory::createBitUnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::BIT}));
    pack->setName("reg");
    pack->setExpectedValue(bytes({1}));
    pack->setCurrentValue(bytes({1}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<UnitTestDataPackage> TestPackageFactory::createInt64UnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::INT64_T}));
    pack->setName("openmb");
    pack->setExpectedValue(bytes({177, 1, 0, 0, 0, 0, 0 ,0}));
    pack->setCurrentValue(bytes({118, 194, 250, 255, 255, 255, 255, 255}));
    pack->setResult(bytes({FAILURE}));
    return pack;

}

QSharedPointer<UnitTestDataPackage> TestPackageFactory::createPtrUnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::PTR}));
    pack->setName("ptr");
    pack->setExpectedValue(bytes({0}));
    pack->setCurrentValue(bytes({0}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<UnitTestDataPackage> TestPackageFactory::createInt16RangeUnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocPackage()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::INT16_T}));
    pack->setName("dfname");
    pack->setCurrentValue(bytes({67 , 0}));
    pack->setLowerValue(bytes({249 , 255}));
    pack->setUpperValue(bytes({226 , 19}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

void ParserTests::insertDataAndRun(QSharedPointer<TransactionDataPackage> transaction)
{
    injecter_.setBuffer(buffer_.data());
    injecter_.inject(transaction);

    root_->startProcessing();

    result_ = root_->getPackage();
}

TEST_F(ParserTests , SimpleLogicTest)
{
    Ptr<TestCaseDataPackage> test_case1{Ptr<TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(factory_.createUint32UnitTest());

    Ptr<TransactionDataPackage> transaction{Ptr<TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    insertDataAndRun(transaction);

    EXPECT_TRUE(*result_ == *transaction);
}

TEST_F(ParserTests , ComplexLogicTest)
{
    Ptr<TestCaseDataPackage> test_case1{Ptr<TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(factory_.createUint32UnitTest());
    test_case1->addUnitTest(factory_.createInt64UnitTest());
    test_case1->addUnitTest(factory_.createInt64UnitTest());
    test_case1->addUnitTest(factory_.createBitUnitTest());
    test_case1->addUnitTest(factory_.createPtrUnitTest());
    test_case1->addUnitTest(factory_.createInt16RangeUnitTest());

    Ptr<TransactionDataPackage> transaction{Ptr<TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    Ptr<TestCaseDataPackage> test_case2{Ptr<TestCaseDataPackage>::create()};
    test_case2->setTestCaseName("test2");
    test_case2->addUnitTest(factory_.createBitUnitTest());
    test_case2->addUnitTest(factory_.createPtrUnitTest());

    transaction->addTestCase(test_case2);

    insertDataAndRun(transaction);

    EXPECT_TRUE(*result_ == *transaction);
}

TEST_F(ParserTests , ChildrenCountTest )
{
    Ptr<TestCaseDataPackage> test_case1{Ptr<TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(factory_.createUint32UnitTest());
    test_case1->addUnitTest(factory_.createInt64UnitTest());
    test_case1->addUnitTest(factory_.createInt64UnitTest());
    test_case1->addUnitTest(factory_.createBitUnitTest());
    test_case1->addUnitTest(factory_.createPtrUnitTest());
    test_case1->addUnitTest(factory_.createInt16RangeUnitTest());

    Ptr<TransactionDataPackage> transaction{Ptr<TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    Ptr<TestCaseDataPackage> test_case2{Ptr<TestCaseDataPackage>::create()};
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
    Ptr<TransactionDataPackage> transaction{Ptr<TransactionDataPackage>::create()};

    EXPECT_NO_FATAL_FAILURE(insertDataAndRun(transaction));
}

TEST_F(ParserTests , EmptyCase )
{
    Ptr<TestCaseDataPackage> test_case1{Ptr<TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    Ptr<TransactionDataPackage> transaction{Ptr<TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    EXPECT_NO_FATAL_FAILURE(insertDataAndRun(transaction));
}
