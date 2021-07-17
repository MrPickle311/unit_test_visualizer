#include "Converter_UnitTests.hpp"
#include <Parser.hpp>
#include <Converter.hpp>
#include <parser/Parser_UnitTests.hpp>
#include <QString>

TEST(ConverterTests , LogicTest)
{
    TestPackageFactory factory;

    TransactionDataPackage parsed_package;

    QSharedPointer<TestCaseDataPackage> test_case{QSharedPointer<TestCaseDataPackage>::create()};
    test_case->setTestCaseName("test1");

    test_case->addUnitTest(factory.createBoolUnitTest());

    parsed_package.addTestCase(test_case);

    Converter converter{parsed_package};

    Transaction transaction;

    transaction = converter.getConverterTransaction();


    EXPECT_TRUE(transaction.cases_.at(0).test_case_name_ == "test1");

    EXPECT_EQ(transaction.cases_.at(0).tests_.at(0).type_descriptor_ , "Bool");

    EXPECT_EQ(transaction.cases_.at(0).tests_.at(0).name_ , "xd()");

    EXPECT_EQ(transaction.cases_.at(0).tests_.at(0).expecteted_value_ , "True");
    EXPECT_EQ(transaction.cases_.at(0).tests_.at(0).current_value_ , "True");
    EXPECT_EQ(transaction.cases_.at(0).tests_.at(0).test_result_ , "Passed");
}
