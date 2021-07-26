#include "../Converter.hpp"
#include <bitset>
#include <QDataStream>
#include <QDebug>
#include <limits>
#include <stdint.h>

namespace backend
{

#define BITS_IN_BYTE 8

QString Int8NumericValueConverter::getValue(const QByteArray& bytes) const
{
    int8_t result = int8_t((unsigned char)(bytes[0]));

    return QString::number(result);
}


QString Int16NumericValueConverter::getValue(const QByteArray& bytes) const
{
    int16_t result = int16_t((unsigned char)(bytes[0])  |
                             (unsigned char)(bytes[1]) << 8);

    return QString::number(result);
}


QString Int32NumericValueConverter::getValue(const QByteArray& bytes) const
{
    int32_t result = int32_t((unsigned char)(bytes[0])       |
                             (unsigned char)(bytes[1]) << 8  |
                             (unsigned char)(bytes[2]) << 16 |
                             (unsigned char)(bytes[3]) << 24 );

    return QString::number(result);
}

QString Int64NumericValueConverter::getValue(const QByteArray& bytes) const
{
    int64_t result = int64_t((uint8_t)(bytes[0])        |
                             (uint8_t)(bytes[1]) << 8   |
                             (uint8_t)(bytes[2]) << 16  |
                             (uint8_t)(bytes[3]) << 24  |
                             (uint64_t)(bytes[4]) << 32 |
                             (uint64_t)(bytes[5]) << 40 |
                             (uint64_t)(bytes[6]) << 48 |
                             (uint64_t)(bytes[7]) << 56 );

    return QString::number(result);
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
        throwIf(pack->getLowerValue().isEmpty() , "Converting error: lower value not specified!");
        throwIf(pack->getUpperValue().isEmpty() , "Converting error: upper value not specified!");

        test.lower_value_ = value_conveter->getValue(pack->getLowerValue());
        test.upper_value_ = value_conveter->getValue(pack->getUpperValue());
    }
    else
    {
        throwIf(pack->getExpectedValue().isEmpty() , "Converting error: expected value not specified!");
        test.expecteted_value_ = value_conveter->getValue(pack->getExpectedValue());
    }
}

QString UnitTestConverter::getTestResult(const QSharedPointer<UnitTestDataPackage>& test)
{
    uint temp{static_cast<uint>(test->getResult().at(0))};

    throwIf(temp > 1 , "Converting error : test_result > 1!");

    if(temp == 1)
        return "Passed";

    return "Failed";
}

UnitTest UnitTestConverter::getUnitTest(const QSharedPointer<UnitTestDataPackage>& test)
{
    UnitTest result;

    throwIf(test->getName().isEmpty() , "Converting error: expression text is empty!");
    throwIf(test->getResult().isEmpty() , "Converting error: test result not specified!");
    throwIf(test->getCurrentValue().isEmpty() , "Converting error: current value not specified!");

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

Converter::Converter():
    transaction_{},
    case_converter_{}
{}

Transaction Converter::getConvertedTransaction()
{   
    for(auto&& test_case : pack_->getCases())
        transaction_.cases_.append(case_converter_.getTestCase(test_case));

    return transaction_;
}

void Converter::reset()
{
    transaction_.cases_.clear();
}

void Converter::setPack(const TransactionDataPackage* pack)
{
    pack_ = pack;
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

    addValue(backend::INT8_T  , QSharedPointer<backend::Int8NumericValueConverter>::create() );
    addValue(backend::INT16_T , QSharedPointer<backend::Int16NumericValueConverter>::create() );
    addValue(backend::INT32_T , QSharedPointer<backend::Int32NumericValueConverter>::create() );
    addValue(backend::INT64_T , QSharedPointer<backend::Int64NumericValueConverter>::create() );

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
