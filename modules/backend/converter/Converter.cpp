#include "../Converter.hpp"
#include <bitset>
#include <QDataStream>

namespace backend
{

#define BITS_IN_BYTE 8

QString SignedNumericValueConverter::getValue(const QByteArray& bytes) const
{
    uint64_t temp_result{0};

    for(int i{0}; i < bytes.size() ; ++i)
        temp_result |=   (uint64_t)( (uint8_t)bytes[i] )  << ( BITS_IN_BYTE * i ) ;
    //                        ^           ^--- conversion to unsigned ( pure byte)
    //                        | - extending type to 64-bit to proper bitwise shifting

    int64_t converted = temp_result;

    return QString::number(converted);
}

QString UnsignedNumericValueConverter::getValue(const QByteArray& bytes) const
{
    uint64_t temp_result{0};

    for(int i{0}; i < bytes.size() ; ++i)
        temp_result |=   (uint64_t)( (uint8_t)bytes[i] )  << ( BITS_IN_BYTE * i ) ;
    //                        ^           ^--- conversion to unsigned ( pure byte)
    //                        | - extending type to 64-bit to proper bitwise shifting

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


TypeDescriptor UnitTestConverter::getDescriptor(const QSharedPointer<UnitTestDataPackage>& test)
{
    return static_cast<TypeDescriptor>(test->getDescriptor().at(0));
}

void UnitTestConverter::convertValues(UnitTest& test, const QSharedPointer<UnitTestDataPackage>& pack)
{
    auto descriptor {getDescriptor(pack)};

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
    interface::Converter{pack},
    transaction_{},
    case_converter_{}
{}

Transaction Converter::getConvertedTransaction()
{
    for(auto&& test_case : pack_.getCases())
        transaction_.cases_.append(case_converter_.getTestCase(test_case));

    return transaction_;
}

void Converter::reset()
{
    transaction_.cases_.clear();
}

}

namespace global
{

template<>
void backend::ValueGenerator::initValues()
{
    auto unsigned_numeric_conv {QSharedPointer<backend::UnsignedNumericValueConverter>::create()};

    addValue(backend::UINT8_T  , unsigned_numeric_conv );
    addValue(backend::UINT16_T , unsigned_numeric_conv );
    addValue(backend::UINT32_T , unsigned_numeric_conv );
    addValue(backend::UINT64_T , unsigned_numeric_conv );

    auto signed_numeric_conv {QSharedPointer<backend::SignedNumericValueConverter>::create()};

    addValue(backend::INT8_T  , signed_numeric_conv );
    addValue(backend::INT16_T , signed_numeric_conv );
    addValue(backend::INT32_T , signed_numeric_conv );
    addValue(backend::INT64_T , signed_numeric_conv );

    addValue(backend::BOOL , QSharedPointer<backend::BoolValueConverter>::create() );
    addValue(backend::CHAR , QSharedPointer<backend::CharValueConverter>::create() );
    addValue(backend::PTR  , QSharedPointer<backend::PtrValueConverter>::create() );
    addValue(backend::BIT  , QSharedPointer<backend::BitValueConverter>::create() );

    is_initialized_ = true;
}

template<>
void backend::StringGenerator::initValues()
{

    addValue(backend::UINT8_T  , "uint8_t" );
    addValue(backend::UINT16_T , "uint16_t" );
    addValue(backend::UINT32_T , "uint32_t" );
    addValue(backend::UINT64_T , "uint64_t" );

    addValue(backend::INT8_T  , "int8_t" );
    addValue(backend::INT16_T , "int16_t" );
    addValue(backend::INT32_T , "int32_t" );
    addValue(backend::INT64_T , "int64_t" );

    addValue(backend::BOOL , "bool" );
    addValue(backend::CHAR , "char" );
    addValue(backend::PTR  , "pointer" );
    addValue(backend::BIT  , "bit" );

    is_initialized_ = true;
}

}
