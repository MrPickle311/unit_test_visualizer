#include "../Converter.hpp"

ValueType NumericValueConverter::getNumericValue(const QByteArray& bytes)
{

}

UnitTestConverter::UnitTestConverter():
    ProgramObject{},
    numeric_converter_{}
{}

uint8_t UnitTestConverter::getDescriptor(const QSharedPointer<UnitTestDataPackage>& test)
{
    return static_cast<uint8_t>(test->getDescriptor().at(0));
}

bool UnitTestConverter::getTestResult(const QSharedPointer<UnitTestDataPackage>& test)
{
    uint temp{static_cast<uint>(test->getResult().at(0))};

    throwIf(temp > 1 , "Test result error : test_result > 1!");

    return static_cast<bool>(temp);
}



UnitTest UnitTestConverter::getUnitTest(const QSharedPointer<UnitTestDataPackage>& test)
{
    UnitTest result;

    result.name_ = test->getName().data();
    result.type_descriptor_ = getDescriptor(test);
    result.test_result_ = getTestResult(test);


}


TestCaseConverter::TestCaseConverter():
    test_converter_{}
{}

TestCase TestCaseConverter::getTestCase(const QSharedPointer<TestCaseDataPackage>& test_case)
{
    TestCase result;

    result.test_case_name_ = test_case->getTestCaseName();

    for(auto&& test : test_case->getTests())
        result.tests_.append(test_converter_.getUnitTest(test));
}

Converter::Converter(const TransactionDataPackage& pack):
    transaction_{},
    pack_{pack},
    case_converter_{}
{}

Transaction Converter::getConverterTransaction()
{
    for(auto&& test_case : pack_.getCases())
        transaction_.cases_.append(case_converter_.getTestCase(test_case));
}


