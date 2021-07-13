#include "TestCaseParser_UnitTests.hpp"

TestCaseParser_UnitTests::TestCaseParser_UnitTests()
{

}

TEST_F(TestCaseParser_UnitTest , LogicTest)
{
    UnitTest_INT16_Range_Builder builder;

    builder.setName("dfname");
    builder.setCurrentValue({67 , 0});
    builder.setLowerValue({249 , 255});
    builder.setUpperValue({226 , 19});
    builder.setTestResult({PASSED});
    //end is appended automatically

    append(builder.createTestData());

    builder.setName("xd");

    append(builder.createTestData());


    UnitTest_PTR_Builder ptr_builder;

    ptr_builder.setName("ptr");
    ptr_builder.setExpectedValue({0});
    ptr_builder.setCurrentValue({0});
    ptr_builder.setTestResult(PASSED);

    append(ptr_builder.createTestData());
    append(ptr_builder.createTestData());

    check();
}
void TestCaseParser_UnitTests::check()
{
    parser_.parseData();

    QList<QByteArrayList> result {parser.getResult()};

    EXPECT_EQ(result.size() , 4);

    CaseTester tester{result};

    tester.run();
    //for(auto&& unit_test_data : result)
    //    unit_test_data.check();
}
