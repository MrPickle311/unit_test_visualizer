#pragma once

#include <QByteArray>
#include <QMap>
#include "PortOperator.hpp"
#include <memory>
#include "parser/ParsedDataPackage.hpp"
#include <variant>

namespace parser
{

//change commands on this
/*
 * class Colors
{
public:
  static const int RED = 1;
  static const int GREEN = 2;
};

class RGB : public Colors
{
  static const int BLUE = 10;
};


class FourColors : public Colors
{
public:
  static const int ORANGE = 100;
  static const int PURPLE = 101;
};
*/

using Code = char;

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
                                PTR	     = 10 ,
                                BIT	     = 11 ,
                                TYPES_COUNT};


struct DataPackage
{
    QByteArrayList parsed_data_;
};

//useful singletons

class TypesSizes
{
private:
    static QMutex mutex_;
    static QMap<TypeDescriptor , int> sizes_;
    static bool sizes_initialized_;
private:
    static void addSize(TypeDescriptor desc , int size);
    static void initSizes();
protected:
    TypesSizes(){};
public:
    static int getSize(TypeDescriptor desc) ;
};

///idea

using AcceptedTypes = std::variant< QSharedPointer<UnitTestDataPackage> ,
                                    QSharedPointer<TestCaseDataPackage> ,
                                    QSharedPointer<TransactionDataPackage> ,
                                    std::monostate >;

class ParserComponent
{
    friend class ComplexParser;
protected:
    ParserComponent*      parent_;
    port::ByteBuffer*     buffer_;
    AcceptedTypes         package_;
    static TypeDescriptor current_type_;
public:
    virtual ParserComponent* getParent();
    virtual void setParent(ParserComponent* newParent);
    virtual void addChild(uint8_t cmd , QSharedPointer<ParserComponent> child);
    virtual bool isComposite() const;
    void setBuffer(port::ByteBuffer* newBuffer);
protected:
    virtual bool parseCommand(AcceptedTypes result) = 0;//if true -> still parsing
    virtual void createPackage();
};

class ComplexParser:
        public ParserComponent,
        public ProgramObject
{
protected:
    QMap<Code , QSharedPointer<ParserComponent>> children_;
    Code                                         commands_count_;
public:
    ComplexParser(Code commands_count);
    virtual bool isComposite() const override;
    virtual void addChild(uint8_t cmd , QSharedPointer<ParserComponent> child) override;
protected:
    void checkCode(Code cmd, std::string class_name);
    void proccessingLoop();
    virtual bool parseCommand(AcceptedTypes result) override;
    virtual void specialPreOperations(AcceptedTypes result);
    virtual void specialPostOperations(AcceptedTypes result);//operations specific to parser
};

///real

class GlobalParser:
        public ComplexParser
{
public:
    GlobalParser();
    virtual void createPackage() override;
    QSharedPointer<TransactionDataPackage> getPackage();
    void startProcessing();
};

class GlobalStartParser:
        public ParserComponent
{
public:
    virtual bool parseCommand(AcceptedTypes result) override;
};

using EmptyParser = GlobalStartParser;

class EndParser:
        public ParserComponent
{
protected:
    virtual bool parseCommand(AcceptedTypes result) override;
};

class TestCaseParser:
        public ComplexParser
{
public:
    TestCaseParser();
protected:
    virtual void createPackage() override;
    virtual void specialPreOperations(AcceptedTypes result) override;
};

class UnitTestParser:
        public ComplexParser
{
public:
    UnitTestParser();
protected:
    virtual void specialPreOperations(AcceptedTypes result) override;
};

class NameParser:
        public ParserComponent
{
protected:
    virtual bool parseCommand(AcceptedTypes result) override;
};

class TypeDescriptorParser:
        public ParserComponent
{
protected:
    virtual bool parseCommand(AcceptedTypes result) override;
};

class ValueParser:
        public ParserComponent
{
protected:
    virtual bool parseCommand(AcceptedTypes result) override;
    virtual void redirectValueBytes(const QByteArray& value_result, QSharedPointer<UnitTestDataPackage>& unit_test ) = 0;
};

class CurrentValueParser:
        public ValueParser
{
public:
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    QSharedPointer<UnitTestDataPackage>& unit_test) override;
};

class ExpectedValueParser:
        public ValueParser
{
protected:
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    QSharedPointer<UnitTestDataPackage>& unit_test) override;
};

class LowerValueParser:
        public ValueParser
{
protected:
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    QSharedPointer<UnitTestDataPackage>& unit_test) override;
};

class UpperValueParser:
        public ValueParser
{
protected:
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    QSharedPointer<UnitTestDataPackage>& unit_test) override;
};

class TestResultParser:
        public ParserComponent
{
protected:
    virtual bool parseCommand(AcceptedTypes result) override;
};

}

