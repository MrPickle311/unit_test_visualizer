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

TEST_F(ConverterTest , ComplexLogicTest)//it fails if it throws
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

    auto unit_test {factory_.createUint32UnitTest()};
    unit_test->setCurrentValue(bytes({149 , 142 , 69}));

    test_case1->addUnitTest(unit_test);

    auto another_unit_test {factory_.createUint64UnitTest()};
    another_unit_test->setCurrentValue(bytes({225 , 143 , 255 , 255}));

    test_case1->addUnitTest(another_unit_test);


    test_case2->setTestCaseName("test2");
    test_case2->addUnitTest(factory_.createBitUnitTest());
    test_case2->addUnitTest(factory_.createPtrUnitTest());

    transaction_pack_.addTestCase(test_case1);
    transaction_pack_.addTestCase(test_case2);

    run();



    EXPECT_STREQ(result_.cases_.at(0).tests_.at(0).current_value_.toStdString().c_str() , "433" );
    EXPECT_STREQ(result_.cases_.at(0).tests_.at(1).current_value_.toStdString().c_str() , "-343434" );
    EXPECT_STREQ(result_.cases_.at(0).tests_.at(2).current_value_.toStdString().c_str() , "-343434" );

    EXPECT_STREQ(result_.cases_.at(0).tests_.at(3).current_value_.toStdString().c_str() ,"Bit is set" );
    EXPECT_STREQ(result_.cases_.at(0).tests_.at(4).current_value_.toStdString().c_str() ,"Is null pointer" );
    EXPECT_STREQ(result_.cases_.at(0).tests_.at(5).current_value_.toStdString().c_str() ,"67" );
    EXPECT_STREQ(result_.cases_.at(0).tests_.at(6).current_value_.toStdString().c_str() ,"4558485" );
    EXPECT_STREQ(result_.cases_.at(0).tests_.at(7).current_value_.toStdString().c_str() ,"4294938593" );

    //second case

    EXPECT_STREQ(result_.cases_.at(1).tests_.at(0).current_value_.toStdString().c_str() , "Bit is set" );
    EXPECT_STREQ(result_.cases_.at(1).tests_.at(1).current_value_.toStdString().c_str() , "Is null pointer" );

}

TEST_F(ConverterTest , EmptyUnitTestTest)//it success if it throws
{
    using namespace parser;

    TestCasePackPtr test_case1{TestCasePackPtr::create()};
    test_case1->setTestCaseName("test1");

    test_case1->addUnitTest(UnitTestPackPtr::create());

    transaction_pack_.addTestCase(test_case1);

    EXPECT_ANY_THROW(run());//i expect here damaged data
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

