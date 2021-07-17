#include "Converter_UnitTests.hpp"
#include <Parser.hpp>
#include <Converter.hpp>
#include <parser/Parser_UnitTests.hpp>
#include <QString>

void ConverterTest::run()
{
    result_ = converter_.getConverterTransaction();
}


TEST_F(ConverterTest , SimpleLogicTest)
{
    using namespace parser;

    TestCasePackPtr test_case{TestCasePackPtr::create()};
    test_case->setTestCaseName("test1");

    test_case->addUnitTest(factory_.createBoolUnitTest());

    transaction_pack_.addTestCase(test_case);

    run();

    EXPECT_TRUE(result_.cases_.at(0).test_case_name_ == "test1");

    EXPECT_EQ(result_.cases_.at(0).tests_.at(0).type_descriptor_ , "bool");

    EXPECT_EQ(result_.cases_.at(0).tests_.at(0).name_ , "xd()");

    EXPECT_EQ(result_.cases_.at(0).tests_.at(0).expecteted_value_ , "True");
    EXPECT_EQ(result_.cases_.at(0).tests_.at(0).current_value_ , "True");
    EXPECT_EQ(result_.cases_.at(0).tests_.at(0).test_result_ , "Passed");
}

TEST_F(ConverterTest , ForceThrowTest)//it fails if it throws
{
    using namespace parser;

    TestCasePackPtr test_case1{TestCasePackPtr::create()};
    test_case1->setTestCaseName("test1");

    TestCasePackPtr test_case2{TestCasePackPtr::create()};
    test_case2->setTestCaseName("test2");

    test_case1->addUnitTest(factory_.createUint32UnitTest());
    test_case1->addUnitTest(factory_.createInt64UnitTest());
    test_case1->addUnitTest(factory_.createInt64UnitTest());
    test_case1->addUnitTest(factory_.createBitUnitTest());
    test_case1->addUnitTest(factory_.createPtrUnitTest());
    test_case1->addUnitTest(factory_.createInt16RangeUnitTest());

    test_case2->setTestCaseName("test2");
    test_case2->addUnitTest(factory_.createBitUnitTest());
    test_case2->addUnitTest(factory_.createPtrUnitTest());

    transaction_pack_.addTestCase(test_case1);
    transaction_pack_.addTestCase(test_case2);

    EXPECT_NO_THROW(run());
}

TEST_F(ConverterTest , EmptyUnitTestTest)//it fails if it throws
{
    using namespace parser;

    TestCasePackPtr test_case1{TestCasePackPtr::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(UnitTestPackPtr::create());

    transaction_pack_.addTestCase(test_case1);

    EXPECT_ANY_THROW(run());
}

TEST_F(ConverterTest , EmptyTestCaseTest)//it fails if it throws
{
    using namespace parser;

    TestCasePackPtr test_case1{TestCasePackPtr::create()};
    test_case1->setTestCaseName("test1");

    transaction_pack_.addTestCase(test_case1);

    EXPECT_NO_THROW(run());
}

TEST_F(ConverterTest , EmptyTransactionTest)//it fails if it throws
{
    EXPECT_NO_THROW(run());
}



ConverterTest::ConverterTest():
    transaction_pack_{},
    converter_{transaction_pack_},
    factory_{}
{}

