#include "TestCaseParser_UnitTests.hpp"

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

    global->addChild(start);
    global->addChild(case_parser);
    global->addChild(end_global);

    Ptr<UnitTestParser> unit_parser{Ptr<UnitTestParser>::create()};
    //Ptr<EndParser> end_case {Ptr<EndParser>::create()};

    case_parser->addChild(unit_parser);
    case_parser->addChild(end_global);

    Ptr<TypeDescriptorParser>  type_parser   {Ptr<TypeDescriptorParser>::create()};
    Ptr<NameParser>            name_parser   {Ptr<NameParser>::create()};
    Ptr<ValueDescriptorParser> value_parser  {Ptr<ValueDescriptorParser>::create()};
    Ptr<TestResultParser>      result_parser {Ptr<TestResultParser>::create()};

    unit_parser->addChild(type_parser);
    unit_parser->addChild(name_parser);
    unit_parser->addChild(value_parser);
    unit_parser->addChild(value_parser);
    unit_parser->addChild(result_parser);
    unit_parser->addChild(Ptr<EmptyParser>::create());//5 - line nmbr -> not implemented yet
    unit_parser->addChild(value_parser);
    unit_parser->addChild(value_parser);
    unit_parser->addChild(end_global);

    buf->appendByte(0);//start

    buf->appendByte(1);//send test case

    buf->appendByte(116);
    buf->appendByte(101);
    buf->appendByte(115);
    buf->appendByte(116);
    buf->appendByte(49);
    buf->appendByte(0);

    buf->appendByte(0);

    buf->appendByte(1);//test case stop

    buf->appendByte(2);//stop

    Ptr<ParsedDataPackage> package_{};

    global->parseCommand(package_);

    EXPECT_STREQ(package_->getBytes().data() , "test1");

}
