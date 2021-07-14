#include "TestCaseParser_UnitTests.hpp"
#include "UnitTestBuilder.hpp"

//TestCaseParser_UnitTests::TestCaseParser_UnitTests()
//{
//
//}
//
//TEST_F(TestCaseParser_UnitTest , LogicTest)
//{
//    UnitTest_INT16_Range_Builder builder;
//
//    builder.setName("dfname");
//    builder.setCurrentValue({67 , 0});
//    builder.setLowerValue({249 , 255});
//    builder.setUpperValue({226 , 19});
//    builder.setTestResult({PASSED});
//    //end is appended automatically
//
//    append(builder.createTestData());
//
//    builder.setName("xd");
//
//    append(builder.createTestData());
//
//
//    UnitTest_PTR_Builder ptr_builder;
//
//    ptr_builder.setName("ptr");
//    ptr_builder.setExpectedValue({0});
//    ptr_builder.setCurrentValue({0});
//    ptr_builder.setTestResult(PASSED);
//
//    append(ptr_builder.createTestData());
//    append(ptr_builder.createTestData());
//
//    check();
//}
//
//
//void TestCaseParser_UnitTests::check()
//{
//    parser_.parseData();
//
//    QList<QByteArrayList> result {parser.getResult()};
//
//    EXPECT_EQ(result.size() , 4);
//
//    CaseTester tester{result};
//
//    tester.run();
//    //for(auto&& unit_test_data : result)
//    //    unit_test_data.check();
//}

template<typename T>
using Ptr = QSharedPointer<T>;

//MAKE A FACTORY

TEST(ParserTests , ComposingTest )
{
    using namespace parser;

    Ptr<port::ByteBuffer> buf{Ptr<port::ByteBuffer>::create()};

    Ptr<GlobalParser> global{Ptr<GlobalParser>::create()};

    global->setBuffer(buf.data());

    Ptr<GlobalStartParser> start{Ptr<GlobalStartParser>::create()};

    Ptr<EndParser> end_global {Ptr<EndParser>::create()};

    Ptr<TestCaseParser> case_parser {Ptr<TestCaseParser>::create()};

    global->addChild(static_cast<uint8_t>(parser::GlobalCommand::START) , start);
    global->addChild(parser::GlobalCommand::SENDING_TEST_CASE, case_parser);
    global->addChild(parser::GlobalCommand::END_ENTIRE_TRANSACTION, end_global);

    Ptr<UnitTestParser> unit_parser{Ptr<UnitTestParser>::create()};
    //Ptr<EndParser> end_case {Ptr<EndParser>::create()};

    case_parser->addChild(parser::TestCaseCommand::SENDING_UNIT_TEST_RESULT , unit_parser);
    case_parser->addChild(parser::TestCaseCommand::END_SENDING_TEST_CASE , end_global);

    Ptr<TypeDescriptorParser>  type_parser   {Ptr<TypeDescriptorParser>::create()};
    Ptr<NameParser>            name_parser   {Ptr<NameParser>::create()};
    Ptr<ValueDescriptorParser> value_parser  {Ptr<ValueDescriptorParser>::create()};
    Ptr<TestResultParser>      result_parser {Ptr<TestResultParser>::create()};

    unit_parser->addChild(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR , type_parser);
    unit_parser->addChild(parser::UnitTestCommand::SENDING_NAME , name_parser);
    unit_parser->addChild(parser::UnitTestCommand::SENDING_CURRENT_VALUE , value_parser);
    unit_parser->addChild(parser::UnitTestCommand::SENDING_EXPECTED_VALUE , value_parser);
    unit_parser->addChild(parser::UnitTestCommand::SENDING_TEST_RESULT , result_parser);
    unit_parser->addChild(parser::UnitTestCommand::SENDING_UPPER_VALUE , value_parser);
    unit_parser->addChild(parser::UnitTestCommand::SENDING_LOWER_VALUE , value_parser);
    unit_parser->addChild(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT , end_global);

    TestCase test_case;
    test_case.setTestCaseName("test1");

    Ptr<UnitTestDataPackage> pack_{Ptr<UnitTestDataPackage>::create()};
    pack_->setDescriptor(TypeDescriptor::BOOL);
    pack_->setName("xd()");
    pack_->setExpectedValue({1});
    pack_->setCurrentValue({1});
    pack_->setResult(1);

    test_case.addUnitTest(pack_);

    pack_->setName("loloo");
    test_case.addUnitTest(pack_);

    Ptr<RangeUnitTestDataPackage> range_pack{Ptr<RangeUnitTestDataPackage>::create()};

    range_pack->setDescriptor(TypeDescriptor::INT16_T);
    range_pack->setName("dfname");
    range_pack->setLowerValue({249 , 255});
    range_pack->setUpperValue({226 , 19});
    range_pack->setResult(1);

    //test_case.addUnitTest(range_pack);

    Transaction transaction;
    transaction.addTestCase(test_case);
    test_case.setTestCaseName("test2");
    transaction.addTestCase(test_case);

    transaction.setBuffer(buf.data());
    transaction.inject();

    Ptr<ParsedDataPackage> package_{};

    global->parseCommand(package_);

    EXPECT_STREQ(package_->getChild(0).getBytes().data() , "test1");
    EXPECT_STREQ(package_->getChild(1).getBytes().data() , "test2");

}
