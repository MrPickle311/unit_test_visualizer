#pragma once

#include <QList>
#include <QString>
#include <variant>
#include "ConvertedDataTypes.hpp"
#include "ParsedDataTypes.hpp"
#include "../global/ProgramObject.hpp"
#include "Interfaces.hpp"

namespace backend
{

/// @brief This is interface for all value converters
class AbstractValueConverter
{
public:
    virtual QString getValue(const QByteArray& bytes) const = 0;
};

/// @brief This converter converts 1-byte value to stringlized int8_t type
class Int8NumericValueConverter:
        public AbstractValueConverter
{
public:
    /// @param bytes Bytes to be converted to numeric value
    /// @return Returns a stringlized numeric value
    virtual QString getValue(const QByteArray& bytes) const override;
};

/// @brief This converter converts 2-byte value to stringlized int16_t type
class Int16NumericValueConverter:
        public AbstractValueConverter
{
public:
    /// @param bytes Bytes to be converted to numeric value
    /// @return Returns a stringlized numeric value
    virtual QString getValue(const QByteArray& bytes) const override;
};

/// @brief This converter converts 4-byte value to stringlized int32_t type
class Int32NumericValueConverter:
        public AbstractValueConverter
{
public:
    /// @param bytes Bytes to be converted to numeric value
    /// @return Returns a stringlized numeric value
    virtual QString getValue(const QByteArray& bytes) const override;
};

/// @brief This converter converts 8-byte value to stringlized int64_t type
class Int64NumericValueConverter:
        public AbstractValueConverter
{
public:
    /// @param bytes Bytes to be converted to numeric value
    /// @return Returns a stringlized numeric value
    virtual QString getValue(const QByteArray& bytes) const override;
};

/// @brief This converter converts several bytes to unsigned numeric value
///
/// To converting unsigned values is only one converterd because signed values
/// have problems with bitwise shifting and type casting
class UnsignedNumericValueConverter:
        public AbstractValueConverter
{
public:
    /// @param bytes Bytes to be converted to numeric value
    /// @return Returns a stringlized numeric value
    virtual QString getValue(const QByteArray& bytes) const override;
};

/// @brief Converts a 1-byte value to a bool type
class BoolValueConverter:
        public AbstractValueConverter
{
public:
    /// @param bytes Byte to be converted to bool value
    /// @return Returns a stringlized bool value
    virtual QString getValue(const QByteArray& bytes) const override;
};

/// @brief Converts a 1-byte value to a pointer type
///
/// 1 byte can represent this that pointer is NULL or not
class PtrValueConverter:
        public AbstractValueConverter
{
public:
    /// @param bytes Byte to be converted to pointer type
    /// @return Returns a stringlized pointer type value
    virtual QString getValue(const QByteArray& bytes) const override;
};

/// @brief Converts a 1-byte value to a bit type
///
/// 1 byte can represent this that bit is set somewhere or not
class BitValueConverter:
        public AbstractValueConverter
{
public:
    /// @param bytes Byte to be converted to bit type
    /// @return Returns a stringlized bit type value
    virtual QString getValue(const QByteArray& bytes) const override;
};

/// @brief Converts a 1-byte value to a char
class CharValueConverter:
        public AbstractValueConverter
{
public:
    /// @param bytes Byte to be converted to a char
    /// @return Returns a char
    virtual QString getValue(const QByteArray& bytes) const override;
};

/// This static generator stores created converter object which can
/// be used by backend::UnitTestConverter to get stringlized values.
/// They can be accesed by passing a TypeDescriptor enum value to getValue() method
/// of this singleton object
/// @see global::StaticGenerator
using ValueGenerator  = global::StaticGenerator<TypeDescriptor , QSharedPointer<backend::AbstractValueConverter>>;

/// This static generator stores stringlized types descriptors.
/// For example when UINT8_T enum value is passed then "uint8_t" string is returned
/// They can be accesed by passing a TypeDescriptor enum value to getValue() method
/// of this singleton object
/// @see global::StaticGenerator
using StringGenerator = global::StaticGenerator<TypeDescriptor , QString>;

}

namespace global
{

template<>
void backend::ValueGenerator::initValues();

template<>
void backend::StringGenerator::initValues();

}

namespace backend
{

/// @brief This class converts UnitTestDataPackage objects to
/// UnitTest object which can be send to QML or JS function
class UnitTestConverter:
        public global::ProgramObject
{
protected:
    /// This method uses value converters to obtain stringilized numeric values
    /// @param test Unit test to which strings should be written
    /// @param pack Parsed unit test package which should be converted
    void              convertValues(UnitTest& test , const QSharedPointer<UnitTestDataPackage>& pack);
    /// This helper method converts parsed type descriptor to the corresponding enum
    /// @param test Unit test package where type descriptor is written
    /// @return Type descriptor converted to enum
    TypeDescriptor    getDescriptor(const QSharedPointer<UnitTestDataPackage>& test);
    /// This helper method converts parsed test result to string form
    /// @param test Unit test package where test result is written
    /// @return Test result converted to string form
    QString           getTestResult(const QSharedPointer<UnitTestDataPackage>& test);
public:
    UnitTestConverter();
    /// This method is used to convert parsed unit test package to UnitTest type
    /// @param test Package to convert
    /// @return Returns converted package
    UnitTest getUnitTest(const QSharedPointer<UnitTestDataPackage>& test);
};

/// @brief This class converts TestCaseDataPackage objects to
/// TestCase object
class TestCaseConverter
{
private:
    UnitTestConverter test_converter_;
public:
    ///Default constructor
    TestCaseConverter();
    /// This method is used to convert parsed test case package to TestCase type
    /// @param test_case Package to convert
    /// @return Returns converted package
    TestCase getTestCase(const QSharedPointer<TestCaseDataPackage>& test_case);
};

/// @brief This class converts TransactionDataPackage objects to
/// Transaction
class Converter:
        public interface::Converter
{
private:
    backend::TransactionDataPackage const * pack_; //cannot change the content
    Transaction                             transaction_;
    TestCaseConverter                       case_converter_;
public:
    /// Default constructor
    Converter();
    /// @return Returns converted transaction by copy
    virtual Transaction getConvertedTransaction()                   override;
    /// Deletes currently stored converted transaction
    virtual void        reset()                                     override;
    /// This method allows to set a new target source which is converted to Transaction
    /// @param pack Pointer to parsed transaction package
    virtual void        setPack(const TransactionDataPackage* pack) override;
};

}
