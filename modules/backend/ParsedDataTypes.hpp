#pragma once

#include <QList>
#include <QSharedPointer>
#include <memory>
#include "../global/StaticGenerator.hpp"

namespace backend
{

using Code = char;

///
enum GlobalCommand : uint8_t{ START					   = 0 ,
                              SENDING_TEST_CASE        = 1 ,
                              END_ENTIRE_TRANSACTION   = 2 ,
                              GLOBAL_COMMAND_COUNT};

enum TestCaseCommand : uint8_t{ SENDING_UNIT_TEST_RESULT = 0 ,
                                END_SENDING_TEST_CASE    = 1 ,
                                TEST_CASE_COMMAND_COUNT};

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

enum TypeDescriptor : uint8_t { UINT8_T  = 0  ,/// a message
                                UINT16_T = 1  ,
                                UINT32_T = 2  ,
                                UINT64_T = 3  ,

                                INT8_T   = 4  ,
                                INT16_T  = 5  ,
                                INT32_T  = 6  ,
                                INT64_T  = 7  ,

                                BOOL     = 8  ,
                                CHAR     = 9  ,
                                PTR	     = 10 ,
                                BIT	     = 11 ,
                                TYPES_COUNT};


using TypesSizes = global::StaticGenerator<TypeDescriptor , int>;

}


namespace backend
{

class UnitTestDataPackage
{
public:
    const QByteArray& getName()          const;
    const QByteArray& getDescriptor()    const;
    const QByteArray& getResult()        const;
    const QByteArray& getCurrentValue()  const;
    const QByteArray& getExpectedValue() const;
    const QByteArray& getLowerValue()    const;
    const QByteArray& getUpperValue()    const;

    void setName(           const QByteArray& newName);
    void setDescriptor(     const QByteArray& newDescriptor);
    void setResult(         const QByteArray& newResult);
    void setCurrentValue(   const QByteArray& newCurrent_value);
    void setExpectedValue(  const QByteArray& newExpected_value);
    void setLowerValue(     const QByteArray& newLower_value);
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
    bool operator==(const UnitTestDataPackage& other) const;
    bool operator!=(const UnitTestDataPackage& other) const;
};

class TestCaseDataPackage
{
public:
    using TestPackPtr = QSharedPointer<UnitTestDataPackage>;
private:
    QList<TestPackPtr> tests_;
    QByteArray         test_case_name_;
public:
    const QByteArray&                   getTestCaseName()                                       const;
    void                                setTestCaseName(const QByteArray& newTest_case_name);
    void                                addUnitTest(TestPackPtr test);
    QSharedPointer<UnitTestDataPackage> getUnitTest(int idx);
    const QList<TestPackPtr>&           getTests()                                              const;
    bool                                operator==(const TestCaseDataPackage& other);
    bool                                operator!=(const TestCaseDataPackage& other);
};

class TransactionDataPackage
{
private:
    QList<QSharedPointer<TestCaseDataPackage>> cases_;
public:
    void                                               addTestCase(QSharedPointer<TestCaseDataPackage> test_case);
    QSharedPointer<TestCaseDataPackage>                getTestCase(int idx);
    const QList<QSharedPointer<TestCaseDataPackage> >& getCases()                                                   const;
    bool                                               operator==(const TransactionDataPackage& other);
    void                                               clear();
};

}
