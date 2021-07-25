#pragma once

#include "ParsedDataTypes.hpp"
#include <variant>
#include "../global/StaticGenerator.hpp"
#include "Interfaces.hpp"

namespace global
{

using backend::TypesSizes;
using backend::TypeDescriptor;

template<>
void TypesSizes::initValues();

}

namespace backend
{
///idea

//aliases

//using interface::AcceptedTypes;

using UnitTestPackPtr    = QSharedPointer<UnitTestDataPackage>;
using TestCasePackPtr    = QSharedPointer<TestCaseDataPackage>;
using TransactionPackPtr = QSharedPointer<TransactionDataPackage>;

class ParserComponent:
        public interface::ParserComponent
{
    friend class ComplexParser;
protected:
    interface::ParserComponent* parent_;
    interface::ByteBuffer*      buffer_;
    AcceptedTypes               package_;
    static TypeDescriptor       current_type_;
public:
    virtual interface::ParserComponent* getParent() override;
    virtual void setParent(interface::ParserComponent* newParent) override;
    virtual void addChild(uint8_t cmd , QSharedPointer<interface::ParserComponent> child) override;
    virtual bool isComposite() const override;
    virtual void setBuffer(interface::ByteBuffer* newBuffer) override;
protected:
    virtual void createPackage() override;
};

class ComplexParser:
        public ParserComponent,
        public global::ProgramObject
{
protected:
    QMap<Code , QSharedPointer<interface::ParserComponent>> children_;
    Code                                         commands_count_;
public:
    ComplexParser(Code commands_count);
    virtual bool isComposite() const override;
    virtual void addChild(uint8_t cmd , QSharedPointer<interface::ParserComponent> child) override;
    virtual void setBuffer(interface::ByteBuffer* newBuffer) override;
protected:
    void checkCode(Code cmd, std::string class_name);
    void proccessingLoop();
    virtual bool parseCommand(AcceptedTypes result) override;
    virtual void specialPreOperations(AcceptedTypes result);
    virtual void specialPostOperations(AcceptedTypes result);//operations specific to parser
};

///real

class RootParser:
        public ComplexParser
{
public:
    RootParser();
    virtual void       createPackage() override;
    virtual TransactionPackPtr getPackage() override;
    virtual void               startProcessing() override;
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
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    UnitTestPackPtr& unit_test ) = 0;
};

class CurrentValueParser:
        public ValueParser
{
public:
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    UnitTestPackPtr& unit_test) override;
};

class ExpectedValueParser:
        public ValueParser
{
protected:
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    UnitTestPackPtr& unit_test) override;
};

class LowerValueParser:
        public ValueParser
{
protected:
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    UnitTestPackPtr& unit_test) override;
};

class UpperValueParser:
        public ValueParser
{
protected:
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    UnitTestPackPtr& unit_test) override;
};

class TestResultParser:
        public ParserComponent
{
protected:
    virtual bool parseCommand(AcceptedTypes result) override;
};

enum class ParserImplementations
{
    FirstImplementation
};

using ReadyParsers  = global::StaticGenerator< ParserImplementations , QSharedPointer<RootParser>>;

}

namespace global
{

template<>
void backend::ReadyParsers::initValues();

}
