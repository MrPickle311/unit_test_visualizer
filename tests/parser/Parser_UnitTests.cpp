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

QSharedPointer<UnitTestDataPackage> ParserTests::allocTest()
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

QSharedPointer<UnitTestDataPackage> ParserTests::createBoolUnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocTest()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::BOOL}));
    pack->setName("xd()");
    pack->setExpectedValue(bytes({1}));
    pack->setCurrentValue(bytes({1}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<UnitTestDataPackage> ParserTests::createUint32UnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocTest()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::UINT32_T}));
    pack->setName("xc");
    pack->setExpectedValue(bytes({177 , 1 , 0 , 0}));
    pack->setCurrentValue(bytes({177 , 1 , 0 , 0}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<UnitTestDataPackage> ParserTests::createBitUnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocTest()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::BIT}));
    pack->setName("reg");
    pack->setExpectedValue(bytes({1}));
    pack->setCurrentValue(bytes({1}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<UnitTestDataPackage> ParserTests::createInt64UnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocTest()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::INT64_T}));
    pack->setName("openmb");
    pack->setExpectedValue(bytes({177, 1, 0, 0, 0, 0, 0 ,0}));
    pack->setCurrentValue(bytes({118, 194, 250, 255, 255, 255, 255, 255}));
    pack->setResult(bytes({FAILURE}));
    return pack;

}

QSharedPointer<UnitTestDataPackage> ParserTests::createPtrUnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocTest()};
    pack->setDescriptor(bytes({parser::TypeDescriptor::PTR}));
    pack->setName("ptr");
    pack->setExpectedValue(bytes({0}));
    pack->setCurrentValue(bytes({0}));
    pack->setResult(bytes({PASSED}));
    return pack;
}

QSharedPointer<UnitTestDataPackage> ParserTests::createInt16RangeUnitTest()
{
    QSharedPointer<UnitTestDataPackage> pack{allocTest()};
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

/*

//void PackageTester::setPackage(const parser::DataPackage& newPackage)
//{
//    package_ = newPackage;
//}
//
//void PackageTester::expectTypeDescriptor(parser::TypeDescriptor desc)
//{
//    EXPECT_EQ(package_.parsed_data_[parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR][0] , desc);//only first byte
//}
//
//void PackageTester::expectName(std::string name)
//{
//    EXPECT_STREQ(package_.parsed_data_[parser::UnitTestCommand::SENDING_NAME].data() , name.c_str());//only first byte
//}
//
//void PackageTester::expectValue(parser::UnitTestCommand cmd ,QList<uint8_t> values)
//{
//    for(int i{0}; i < values.size() ; ++i)
//        EXPECT_EQ(static_cast<uint8_t>(package_.parsed_data_[cmd][i]) , values[i]);
//}

//void PackageTester::expectCurrentValue(QList<uint8_t> values)
//{
//    expectValue(parser::UnitTestCommand::SENDING_CURRENT_VALUE , values);
//}
//
//void PackageTester::expectLowerValue(QList<uint8_t> values)
//{
//    expectValue(parser::UnitTestCommand::SENDING_LOWER_VALUE , values);
//}
//
//void PackageTester::expectUpperValue(QList<uint8_t> values)
//{
//    expectValue(parser::UnitTestCommand::SENDING_UPPER_VALUE , values);
//}
//
//void PackageTester::expectExpectedValue(QList<uint8_t> values)
//{
//    expectValue(parser::UnitTestCommand::SENDING_EXPECTED_VALUE , values);
//}

//void PackageTester::expectTestResult(TestResult result )
//{
//    EXPECT_EQ(package_.parsed_data_[parser::UnitTestCommand::SENDING_TEST_RESULT][0] , result);
//}
//
/////
//
//void AbstractLocalParser_UnitTests::appendName(std::string name)
//{
//    name_ = name;
//}
//
//void AbstractLocalParser_UnitTests::appendType(parser::TypeDescriptor desc)
//{
//    descriptor_ = desc;
//}
//
//void AbstractLocalParser_UnitTests::appendExpectedValue(QList<uint8_t> values)
//{
//    expected_value_ = values;
//}
//
//void AbstractLocalParser_UnitTests::appendCurrentValue(QList<uint8_t> values)
//{
//    current_value_ = values;
//}
//
//void AbstractLocalParser_UnitTests::appendTestResult(TestResult result)
//{
//    result_ = result;
//}
//
//void AbstractLocalParser_UnitTests::appendLowerValue(QList<uint8_t> value)
//{
//    lower_value_ = value;
//}
//
//void AbstractLocalParser_UnitTests::appendUpperValue(QList<uint8_t> value)
//{
//    upper_value_ = value;
//}
//
//void AbstractLocalParser_UnitTests::appendEnd(){} //empty
//
/////
//
//LocalParser_UnitTests::LocalParser_UnitTests():
//    buffer_{},
//    local_parser_(&buffer_),
//    test_body_{}
//{}
//
//void LocalParser_UnitTests::appendCode(uint8_t code)
//{
//    buffer_.appendByte(code);
//}
//
//void LocalParser_UnitTests::appendName(std::string name)
//{
//    test_body_.appendName(name);
//
//    appendCode(parser::UnitTestCommand::SENDING_NAME);
//    for(auto&& c : name)
//        appendCode(c);
//    appendCode('\0');
//}
//
//void LocalParser_UnitTests::appendType(parser::TypeDescriptor desc)
//{
//    test_body_.appendType(desc);
//
//    appendCode(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR);
//    appendCode(desc);
//}
//
//void LocalParser_UnitTests::appendTestResult(TestResult result)
//{
//    test_body_.appendTestResult(result);
//
//    appendCode(parser::UnitTestCommand::SENDING_TEST_RESULT);
//    appendCode(result);
//}
//
//void LocalParser_UnitTests::appendSeveralCodes(QList<uint8_t> codes)
//{
//    for(auto&& value : codes)
//        appendCode(value);
//}
//
//void LocalParser_UnitTests::appendExpectedValue(QList<uint8_t> values)
//{
//    test_body_.appendExpectedValue(values);
//
//    appendCode(parser::UnitTestCommand::SENDING_EXPECTED_VALUE);
//    appendSeveralCodes(values);
//}
//
//void LocalParser_UnitTests::appendCurrentValue(QList<uint8_t> values)
//{
//    test_body_.appendCurrentValue(values);
//
//    appendCode(parser::UnitTestCommand::SENDING_CURRENT_VALUE);
//    appendSeveralCodes(values);
//}
//
//void LocalParser_UnitTests::appendLowerValue(QList<uint8_t> value)
//{
//    test_body_.appendLowerValue(value);
//
//    appendCode(parser::UnitTestCommand::SENDING_LOWER_VALUE);
//    appendSeveralCodes(value);
//}
//
//void LocalParser_UnitTests::appendUpperValue(QList<uint8_t> value)
//{
//    test_body_.appendUpperValue(value);
//
//    appendCode(parser::UnitTestCommand::SENDING_UPPER_VALUE);
//    appendSeveralCodes(value);
//}
//
//void LocalParser_UnitTests::appendEnd()
//{
//    test_body_.appendEnd();
//
//    appendCode(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT);
//}
//
//void LocalParser_UnitTests::fillPackage()
//{
//    local_parser_.parseData();
//
//    expectParserReady();
//
//    package_tester_.setPackage(local_parser_.getParsedPackage());
//}
//
//void LocalParser_UnitTests::checkCommon()
//{
//    package_tester_.expectTypeDescriptor(test_body_.descriptor_);
//    package_tester_.expectName(test_body_.name_);
//    package_tester_.expectCurrentValue(test_body_.current_value_);
//    package_tester_.expectTestResult(test_body_.result_);
//}
//
//void LocalParser_UnitTests::checkEqual()
//{
//    fillPackage();
//    checkCommon();
//
//    package_tester_.expectExpectedValue(test_body_.expected_value_);
//    //here parser is empty
//
//    expectParserNotReady();
//    expectEmptyResult();
//}
//
//void LocalParser_UnitTests::checkInRange()
//{
//    fillPackage();
//    checkCommon();
//
//    package_tester_.expectLowerValue(test_body_.lower_value_);
//    package_tester_.expectUpperValue(test_body_.upper_value_);
//
//    expectParserNotReady();
//    expectEmptyResult();
//}
//
//void LocalParser_UnitTests::expectParserReady() const
//{
//    EXPECT_TRUE(local_parser_.packageReady());
//}
//
//void LocalParser_UnitTests::expectParserNotReady() const
//{
//    EXPECT_FALSE(local_parser_.packageReady());
//}
//
//void LocalParser_UnitTests::expectEmptyResult() const
//{
//    EXPECT_TRUE(local_parser_.isEmptyResult());
//}
//
//void LocalParser_UnitTests::expectNotEmptyResult() const
//{
//    EXPECT_FALSE(local_parser_.isEmptyResult());
//}
//
//TEST_F(LocalParser_UnitTests , DependencyTest)
//{
//    EXPECT_NE(local_parser_.getBuffer() , nullptr);
//}
*/

TEST_F(ParserTests , SimpleLogicTest)
{
    Ptr<TestCaseDataPackage> test_case1{Ptr<TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(createUint32UnitTest());

    Ptr<TransactionDataPackage> transaction{Ptr<TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    insertDataAndRun(transaction);

    EXPECT_TRUE(*result_ == *transaction);
}

TEST_F(ParserTests , ComplexLogicTest)
{
    Ptr<TestCaseDataPackage> test_case1{Ptr<TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(createUint32UnitTest());
    test_case1->addUnitTest(createInt64UnitTest());
    test_case1->addUnitTest(createInt64UnitTest());
    test_case1->addUnitTest(createBitUnitTest());
    test_case1->addUnitTest(createPtrUnitTest());
    test_case1->addUnitTest(createInt16RangeUnitTest());

    Ptr<TransactionDataPackage> transaction{Ptr<TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    Ptr<TestCaseDataPackage> test_case2{Ptr<TestCaseDataPackage>::create()};
    test_case2->setTestCaseName("test2");
    test_case2->addUnitTest(createBitUnitTest());
    test_case2->addUnitTest(createPtrUnitTest());

    transaction->addTestCase(test_case2);

    insertDataAndRun(transaction);

    EXPECT_TRUE(*result_ == *transaction);
}

TEST_F(ParserTests , ChildrenCountTest )
{
    Ptr<TestCaseDataPackage> test_case1{Ptr<TestCaseDataPackage>::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(createUint32UnitTest());
    test_case1->addUnitTest(createInt64UnitTest());
    test_case1->addUnitTest(createInt64UnitTest());
    test_case1->addUnitTest(createBitUnitTest());
    test_case1->addUnitTest(createPtrUnitTest());
    test_case1->addUnitTest(createInt16RangeUnitTest());

    Ptr<TransactionDataPackage> transaction{Ptr<TransactionDataPackage>::create()};
    transaction->addTestCase(test_case1);

    Ptr<TestCaseDataPackage> test_case2{Ptr<TestCaseDataPackage>::create()};
    test_case2->setTestCaseName("test2");
    test_case2->addUnitTest(createBitUnitTest());
    test_case2->addUnitTest(createPtrUnitTest());

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

#include <gtest/gtest.h>

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
