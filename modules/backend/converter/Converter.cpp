#include "../Converter.hpp"

#define BITS_IN_BYTE 8

QString SignedNumericValueConverter::getValue(const QByteArray& bytes) const
{
    int64_t temp_result{0};

    for(int i{0}; i < bytes.size() ; ++i)
        temp_result |= static_cast<int64_t>(bytes[i]) << BITS_IN_BYTE * i;

    return QString::number(temp_result);
}

QString UnsignedNumericValueConverter::getValue(const QByteArray& bytes) const
{
    uint64_t temp_result{0};

    for(int i{0}; i < bytes.size() ; ++i)
        temp_result |= static_cast<uint64_t>(bytes[i]) << BITS_IN_BYTE * i;

    return QString::number(temp_result);
}

QString BoolValueConverter::getValue(const QByteArray& bytes) const
{
    uint result {static_cast<uint>(bytes.at(0))};

    if(result == 0)
        return "False";
    return "True";
}

QString PtrValueConverter::getValue(const QByteArray& bytes) const
{
    uint result {static_cast<uint>(bytes.at(0))};

    if(result == 0)
        return "Is null pointer";
    return "Is not null pointer";
}

QString BitValueConverter::getValue(const QByteArray& bytes) const
{
    uint result {static_cast<uint>(bytes.at(0))};

    if(result == 0)
        return "Bit is not set";
    return "Bit is set";
}

QString CharValueConverter::getValue(const QByteArray& bytes) const
{
    return QString{bytes.at(0)};
}

UnitTestConverter::UnitTestConverter():
    ProgramObject{}
{}


parser::TypeDescriptor UnitTestConverter::getDescriptor(const QSharedPointer<UnitTestDataPackage>& test)
{
    return static_cast<parser::TypeDescriptor>(test->getDescriptor().at(0));
}

void UnitTestConverter::convertValues(UnitTest& test, const QSharedPointer<UnitTestDataPackage>& pack)
{
    parser::TypeDescriptor descriptor {getDescriptor(pack)};

    auto value_conveter {ValueGenerator::getValue(descriptor)};

    test.current_value_ = value_conveter->getValue(pack->getCurrentValue());

    test.type_descriptor_ = StringGenerator::getValue(descriptor);

    if(pack->getExpectedValue().isEmpty())
    {
        throwIf(pack->getLowerValue().isEmpty() , "Error: lower value not specified!");
        throwIf(pack->getUpperValue().isEmpty() , "Error: upper value not specified!");

        test.lower_value_ = value_conveter->getValue(pack->getLowerValue());
        test.upper_value_ = value_conveter->getValue(pack->getUpperValue());
    }
    else
    {
        throwIf(pack->getExpectedValue().isEmpty() , "Error: expected value not specified!");
        test.expecteted_value_ = value_conveter->getValue(pack->getExpectedValue());
    }
}

QString UnitTestConverter::getTestResult(const QSharedPointer<UnitTestDataPackage>& test)
{
    uint temp{static_cast<uint>(test->getResult().at(0))};

    throwIf(temp > 1 , "Test result error : test_result > 1!");

    if(temp == 1)
        return "Passed";

    return "Failed";
}

UnitTest UnitTestConverter::getUnitTest(const QSharedPointer<UnitTestDataPackage>& test)
{
    UnitTest result;

    throwIf(test->getName().isEmpty() , "Error: expression text is empty!");
    throwIf(test->getResult().isEmpty() , "Error: test result not specified!");
    throwIf(test->getCurrentValue().isEmpty() , "Error: current value not specified!");

    result.name_ = test->getName().data();
    result.test_result_ = getTestResult(test);

    convertValues(result , test);

    return result;
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

    return result;
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

    return transaction_;
}

template<>
void Generator<QSharedPointer<AbstractValueConverter>>::initValues()
{
    auto unsigned_numeric_conv {QSharedPointer<UnsignedNumericValueConverter>::create()};

    addValue(parser::UINT8_T  , unsigned_numeric_conv );
    addValue(parser::UINT16_T , unsigned_numeric_conv );
    addValue(parser::UINT32_T , unsigned_numeric_conv );
    addValue(parser::UINT64_T , unsigned_numeric_conv );

    auto signed_numeric_conv {QSharedPointer<SignedNumericValueConverter>::create()};

    addValue(parser::INT8_T  , signed_numeric_conv );
    addValue(parser::INT16_T , signed_numeric_conv );
    addValue(parser::INT32_T , signed_numeric_conv );
    addValue(parser::INT64_T , signed_numeric_conv );

    addValue(parser::BOOL , QSharedPointer<BoolValueConverter>::create() );
    addValue(parser::CHAR , QSharedPointer<CharValueConverter>::create() );
    addValue(parser::PTR  , QSharedPointer<PtrValueConverter>::create() );
    addValue(parser::BIT  , QSharedPointer<BitValueConverter>::create() );

    is_initialized_ = true;
}

template<>
void Generator<QString>::initValues()
{

    addValue(parser::UINT8_T  , "uint8_t" );
    addValue(parser::UINT16_T , "uint16_t" );
    addValue(parser::UINT32_T , "uint32_t" );
    addValue(parser::UINT64_T , "uint64_t" );

    addValue(parser::INT8_T  , "int8_t" );
    addValue(parser::INT16_T , "int16_t" );
    addValue(parser::INT32_T , "int32_t" );
    addValue(parser::INT64_T , "int64_t" );

    addValue(parser::BOOL , "bool" );
    addValue(parser::CHAR , "char" );
    addValue(parser::PTR  , "pointer" );
    addValue(parser::BIT  , "bit" );

    is_initialized_ = true;
}
