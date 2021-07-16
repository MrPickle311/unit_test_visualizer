#include "Converter_UnitTests.hpp"
#include <Parser.hpp>
#include <Converter.hpp>
#include <parser/Parser_UnitTests.hpp>


TEST_F(ConverterTests , LogicTest)
{
    TestPackageFactory factory;

    TransactionDataPackage parsed_package;

    QSharedPointer<TestCaseDataPackage> test_case;
    test_case->setTestCaseName("test1");

    test_case->addUnitTest(factory.createBoolUnitTest());

    Converter converter;
    converter.setTarget(parsed_package);

    Transaction transaction;

    transaction = converter.getConverterTransaction();


    EXPECT_STREQ(transaction.cases_.at(0).test_case_name_ , "test1");

    EXPECT_EQ(transaction.cases_.at(0).tests_.at(0).type_descriptor_ , parser::TypeDescriptor::BOOL);

    EXPECT_EQ(transaction.cases_.at(0).tests_.at(0).name_ , "xd()");

    EXPECT_EQ(transaction.cases_.at(0).tests_.at(0).expecteted_value_ , 1);
    EXPECT_EQ(transaction.cases_.at(0).tests_.at(0).current_value_ , 1);
    EXPECT_EQ(transaction.cases_.at(0).tests_.at(0).test_result_ , 1);

    EXPECT_EQ(transaction , *parsed_package);
}
