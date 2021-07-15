#include "TestCaseParser_UnitTests.hpp"
#include "UnitTestBuilder.hpp"

#include <variant>

QByteArray bytes(QList<uint8_t> byte_lits)
{
    QByteArray result;
    for(auto&& byte : byte_lits)
        result.append(byte);
    return  result;
}

template<typename T>
using Ptr = QSharedPointer<T>;

//MAKE A FACTORY

TEST(ParserTests , ComposingTest )
{
    using namespace parser;

    Ptr<port::ByteBuffer> buf{Ptr<port::ByteBuffer>::create()};

    Ptr<GlobalParser> global{Ptr<GlobalParser>::create()};

    global->setBuffer(buf.data());

    Ptr<EndParser> end {Ptr<EndParser>::create()};

    Ptr<TestCaseParser> case_parser {Ptr<TestCaseParser>::create()};

    global->addChild(parser::GlobalCommand::START , Ptr<GlobalStartParser>::create());
    global->addChild(parser::GlobalCommand::SENDING_TEST_CASE, case_parser);
    global->addChild(parser::GlobalCommand::END_ENTIRE_TRANSACTION, end);

    Ptr<UnitTestParser> unit_parser{Ptr<UnitTestParser>::create()};

    case_parser->addChild(parser::TestCaseCommand::SENDING_UNIT_TEST_RESULT , unit_parser);
    case_parser->addChild(parser::TestCaseCommand::END_SENDING_TEST_CASE , end);

    unit_parser->addChild(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR , Ptr<TypeDescriptorParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_NAME , Ptr<NameParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_CURRENT_VALUE , Ptr<CurrentValueParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_EXPECTED_VALUE , Ptr<ExpectedValueParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_TEST_RESULT , Ptr<TestResultParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_UPPER_VALUE , Ptr<UpperValueParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::SENDING_LOWER_VALUE , Ptr<LowerValueParser>::create());
    unit_parser->addChild(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT , end);

    Ptr<TestCase> test_case{Ptr<TestCase>::create()};
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

    Ptr<Transaction> transaction{Ptr<Transaction>::create()};
    transaction->addTestCase(test_case);
    test_case->setTestCaseName("test2");
    transaction->addTestCase(test_case);

    TransactionInjecter injecter;

    injecter.setBuffer(buf.data());
    injecter.inject(transaction);

    Ptr<ParsedDataPackage> package_{};

    Ptr<Transaction> transaction2;

    global->parseCommand(transaction2);

    EXPECT_STREQ(package_->getChild(0).getBytes().data() , "test1");
    EXPECT_STREQ(package_->getChild(1).getBytes().data() , "test2");

}

//void ff(AcceptedTypes types)
//{
//    auto package {std::get<Ptr<UnitTestDataPackage>>(types)};
//
//    package->setName("XDXDXDXD");
//}
//
//void gg(AcceptedTypes test_case , AcceptedTypes test, AcceptedTypes transaction)
//{
//    std::get<Ptr<TestCase>>(test_case)->addUnitTest(std::get<Ptr<UnitTestDataPackage>>(test));
//
//    std::get<Ptr<Transaction>>(transaction)->addTestCase(std::get<Ptr<TestCase>>(test_case));
//}
//
//TEST(XD ,xd)
//{
//    Ptr<UnitTestDataPackage> pck{Ptr<UnitTestDataPackage>::create()};
//
//    ff(pck);
//
//    Ptr<TestCase> test_case{Ptr<TestCase>::create()};
//
//    Ptr<Transaction> transaction{Ptr<Transaction>::create()};
//
//    gg(test_case , pck , transaction );
//
//    qDebug() << pck->getName().data();
//
//
//}











