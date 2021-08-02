#pragma once

#include <QList>
#include <QSharedPointer>
#include <memory>
#include "../global/StaticGenerator.hpp"

namespace backend
{

using Code = char;

/// These commands are used to maintain root parser
enum GlobalCommand : uint8_t{ START					   = 0 ,  ///< Start
                              SENDING_TEST_CASE        = 1 ,  ///< A test case arriving
                              END_ENTIRE_TRANSACTION   = 2 ,  ///< A transaction package is delivered
                              GLOBAL_COMMAND_COUNT};

/// These commands are used to maintain test case parser
enum TestCaseCommand : uint8_t{ SENDING_UNIT_TEST_RESULT = 0 ,///< A unit test arriving
                                END_SENDING_TEST_CASE    = 1 ,///< A unit test package is delivered
                                TEST_CASE_COMMAND_COUNT};

/// These commands are used to maintain unit test parser
enum UnitTestCommand : uint8_t{ SENDING_TYPE_DESCRIPTOR 	 = 0 ,
                                SENDING_NAME				 = 1 ,
                                SENDING_CURRENT_VALUE		 = 2 ,
                                SENDING_EXPECTED_VALUE		 = 3 ,
                                SENDING_TEST_RESULT			 = 4 ,
                                SENDING_LINE_NMBR			 = 5 ,
                                SENDING_LOWER_VALUE          = 6 ,
                                SENDING_UPPER_VALUE          = 7 ,
                                END_SENDING_UNIT_TEST_RESULT = 8 ,
                                COMMANDS_COUNT};

/// Types descriptors. They are used to obtain size of incoming unit test package
enum TypeDescriptor : uint8_t { UINT8_T  = 0  ,
                                UINT16_T = 1  ,
                                UINT32_T = 2  ,
                                UINT64_T = 3  ,

                                INT8_T   = 4  ,
                                INT16_T  = 5  ,
                                INT32_T  = 6  ,
                                INT64_T  = 7  ,

                                BOOL     = 8  ,
                                CHAR     = 9  ,
                                PTR	     = 10 , ///< A pointer , NULL or not
                                BIT	     = 11 , ///< A bit , which is set at current position or not
                                TYPES_COUNT};


/// This static generator provides sizes ( in bytes ) numeric values
/// of types marked in TypeDescriptor enum
using TypesSizes = global::StaticGenerator<TypeDescriptor , int>;

}


namespace backend
{

/// @brief This is parsed unit test package
///
/// Unit test parser creates a package of this type and
/// loads parsed bytes to it
class UnitTestDataPackage
{
public:
    /// @return Returns current expression name
    const QByteArray& getName()          const;
    /// @return Returns current type descriptor
    const QByteArray& getDescriptor()    const;
    /// @return Returns current test result
    const QByteArray& getResult()        const;
    /// @return Returns current value
    const QByteArray& getCurrentValue()  const;
    /// @return Returns currently set expected value
    const QByteArray& getExpectedValue() const;
    /// @return Returns currently set lower value
    const QByteArray& getLowerValue()    const;
    /// @return Returns currently set upper value
    const QByteArray& getUpperValue()    const;

    /// @param newName A new expression name to set
    void setName(           const QByteArray& newName);
    /// @param newDescriptor A new expression type descriptor to set
    void setDescriptor(     const QByteArray& newDescriptor);
    /// @param newResult A new test result to set
    void setResult(         const QByteArray& newResult);
    /// @param newCurrent_value A new current value to set
    void setCurrentValue(   const QByteArray& newCurrent_value);
    /// @param newExpected_value A new expected value to set
    void setExpectedValue(  const QByteArray& newExpected_value);
    /// @param newLower_value A new lower value to set
    void setLowerValue(     const QByteArray& newLower_value);
    /// @param newUpper_value A new upper value to set
    void setUpperValue(     const QByteArray& newUpper_value);
protected:
    QByteArray  name_;
    QByteArray  descriptor_;
    QByteArray  result_;
    QByteArray  current_value_;
    QByteArray  expected_value_;
    QByteArray  lower_value_;
    QByteArray  upper_value_;
public:
    /// @return Returns true if members of this and other are equal
    /// @param other Package to compare
    bool operator==(const UnitTestDataPackage& other) const;
    /// @return Returns true if members of this and other are not equal
    /// @param other Package to compare
    bool operator!=(const UnitTestDataPackage& other) const;
};

/// @brief This is parsed test case package
///
/// Test case parser creates a package of this type and
/// loads parsed test case name to it. Unit test
/// parser loads unit test packages to it
class TestCaseDataPackage
{
public:
    using TestPackPtr = QSharedPointer<UnitTestDataPackage>;
private:
    /// List of unit tests
    QList<TestPackPtr> tests_;
    /// Parsed test case name
    QByteArray         test_case_name_;
public:
    /// @return Returns currently set test case name
    const QByteArray&                   getTestCaseName()                                       const;
    /// @param newTest_case_name A new test case name to set
    void                                setTestCaseName(const QByteArray& newTest_case_name);
    /// Appends a new unit test parsed package to the list
    /// @param test A new unit test parsed package smart pointer to append
    void                                addUnitTest(TestPackPtr test);
    /// @return Returns smart pointer to a unit test
    /// @param idx Index which indicates a package position
    QSharedPointer<UnitTestDataPackage> getUnitTest(int idx);
    /// @return Returns const reference to the list of unit test parsed packages
    const QList<TestPackPtr>&           getTests()                                              const;
    /// @return Returns true if this package list is equal to other list and test case names
    /// are equal
    /// @param other Package to compare
    bool                                operator==(const TestCaseDataPackage& other);
    /// @return Returns true if this package list is not equal to other list or test case names
    /// are not equal
    /// @param other Package to compare
    bool                                operator!=(const TestCaseDataPackage& other);
};

/// @brief This is parsed transaction package
///
/// Root parser creates a package of this type.
/// Test case parser loads parsed test cases packages to it.
class TransactionDataPackage
{
private:
    /// Parsed test cases packs
    QList<QSharedPointer<TestCaseDataPackage>> cases_;
public:
    /// This method allows to add a new test case package to transaction package
    /// @param test_case Test case package to add
    void                                               addTestCase(QSharedPointer<TestCaseDataPackage> test_case);
    /// @return Returns a smart pointer to a test case package
    QSharedPointer<TestCaseDataPackage>                getTestCase(int idx);
    /// @return Returns const reference to the list of test case parsed packages
    const QList<QSharedPointer<TestCaseDataPackage> >& getCases()                                                   const;
    /// @return Returns true if all test case packages are equal
    bool                                               operator==(const TransactionDataPackage& other);
    /// This method clears test case package list
    void                                               clear();
};

}
