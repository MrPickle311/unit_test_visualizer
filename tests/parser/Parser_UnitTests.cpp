#include "Parser_UnitTests.hpp"
#include <QObject>

template<typename T>
using Ptr = QSharedPointer<T>;

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
//
//TEST_F(LocalParser_UnitTests , BoolVariableTest)
//{
//    appendType(parser::TypeDescriptor::BOOL);
//    appendName("xd()");
//    appendExpectedValue({1});
//    appendCurrentValue({1});
//    appendTestResult(PASSED);
//    appendEnd();
//
//    checkEqual();
//}
//
//TEST_F(LocalParser_UnitTests , Uint32VariableTest)
//{
//    appendType(parser::TypeDescriptor::UINT32_T);
//    appendName("xc");
//    appendExpectedValue({177 , 1 , 0 , 0});
//    appendCurrentValue({177 , 1 , 0 , 0});
//    appendTestResult(PASSED);
//    appendEnd();
//
//    checkEqual();
//}
//
//TEST_F(LocalParser_UnitTests , BitSetTest)
//{
//    appendType(parser::TypeDescriptor::BIT);
//    appendName("reg");
//    appendExpectedValue({1});
//    appendCurrentValue({1});
//    appendTestResult(PASSED);
//    appendEnd();
//
//    checkEqual();
//}
//
//TEST_F(LocalParser_UnitTests , Int64VariableTest)
//{
//    appendType(parser::TypeDescriptor::INT64_T);
//    appendName("openmb");
//    appendExpectedValue({177, 1, 0, 0, 0, 0, 0 ,0});
//    appendCurrentValue({118, 194, 250, 255, 255, 255, 255, 255});
//    appendTestResult(FAILURE);
//    appendEnd();
//
//    checkEqual();
//}
//
//TEST_F(LocalParser_UnitTests , PtrVariableTest)
//{
//    appendType(parser::TypeDescriptor::PTR);
//    appendName("ptr");
//    appendExpectedValue({0});
//    appendCurrentValue({0});
//    appendTestResult(PASSED);
//    appendEnd();
//
//    checkEqual();
//}
//
//TEST_F(LocalParser_UnitTests , Int16RangeVariableTest)
//{
//    appendType(parser::TypeDescriptor::INT16_T);
//    appendName("dfname");
//    appendCurrentValue({67 , 0});
//    appendLowerValue({249 , 255});
//    appendUpperValue({226 , 19});
//    appendTestResult(PASSED);
//    appendEnd();
//
//    checkInRange();
//}
//

QByteArray bytes(QList<uint8_t> byte_lits)
{
    QByteArray result;
    for(auto&& byte : byte_lits)
        result.append(byte);
    return  result;
}

//MAKE A FACTORY

TEST_F(ParserTests , LogicTest )
{
    using namespace parser;

    Ptr<TestCaseDataPackage> test_case{Ptr<TestCaseDataPackage>::create()};
    test_case->setTestCaseName("test1");

    Ptr<UnitTestDataPackage> pack_{Ptr<UnitTestDataPackage>::create()};
    pack_->setDescriptor(bytes({TypeDescriptor::BOOL}));
    pack_->setName("xd()");
    pack_->setExpectedValue(bytes({1}));
    pack_->setCurrentValue(bytes({1}));
    pack_->setResult(bytes({1}));

    test_case->addUnitTest(pack_);

    pack_->setName("loloo");
    test_case->addUnitTest(pack_);

    Ptr<UnitTestDataPackage> range_pack{Ptr<UnitTestDataPackage>::create()};

    range_pack->setDescriptor(bytes({TypeDescriptor::INT16_T}));
    range_pack->setName("dfname");
    range_pack->setCurrentValue(bytes({67 , 0}));
    range_pack->setLowerValue(bytes({249 , 255}));
    range_pack->setUpperValue(bytes({226 , 19}));
    range_pack->setResult(bytes({1}));

    test_case->addUnitTest(range_pack);

    Ptr<TransactionDataPackage> transaction{Ptr<TransactionDataPackage>::create()};
    transaction->addTestCase(test_case);
    test_case->setTestCaseName("test2");
    transaction->addTestCase(test_case);

    TransactionInjecter injecter;

    injecter.setBuffer(buffer_.data());
    injecter.inject(transaction);

    root_->startProcessing();

    result_ = root_->getPackage();

    //EXPECT_STREQ(package_->getChild(0).getBytes().data() , "test1");
    //EXPECT_STREQ(package_->getChild(1).getBytes().data() , "test2");

}
