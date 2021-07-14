#pragma once

#include <QByteArray>
#include <QStateMachine>
#include <QState>
#include <QSignalTransition>
#include "PortOperator.hpp"
#include <memory>
#include "parser/ParsedDataPackage.hpp"

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


struct Command
{
    Code command_code_;
};

//replace with using DataPackage = QByteArrayList;
struct DataPackage
{
    QByteArrayList parsed_data_;
};

class AbstractProcessor
{
protected:
    static TypeDescriptor current_descriptor_;//shared between processors
public:
    virtual ~AbstractProcessor(){}
    virtual QByteArray process(port::ByteBuffer* buffer) = 0;
};

class Processor:
        public AbstractProcessor,
        public ProgramObject
{};

class TypeDescriptorProcessor:
        public Processor
{
public:
    virtual QByteArray process(port::ByteBuffer* buffer) override;
};

class NameProcessor:
        public Processor
{
public:
    virtual QByteArray process(port::ByteBuffer* buffer) override;
};

class ValueProcessor:
        public Processor
{
public:
    virtual QByteArray process(port::ByteBuffer* buffer) override;
};

class TestResultProcessor:
        public Processor
{
public:
    virtual QByteArray process(port::ByteBuffer* buffer) override;
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

class Processors
{
private:
    static QMutex mutex_;
    static QMap<UnitTestCommand , std::shared_ptr<Processor>> processors_;
    static bool processors_initialized_;
private:
    template<typename T>
    static void addProcessor(UnitTestCommand code);
    static void initProcessors();
protected:
    Processors(){};
public:
    static Processor* getProcessor(UnitTestCommand cmd);
};


//interface for every kind of parser
//pure abstraction , no data
class AbstractParser
{
private :
    virtual void parseCommand(Code cmd) = 0;
public:
    virtual bool packageReady() const = 0;
    virtual DataPackage getParsedPackage() = 0;
    virtual bool isEmptyResult() const = 0;
};

//abstraction for a byte parser
class AbstractByteParser:
        public AbstractParser
{
protected:
    port::ByteBuffer* buffer_;
public:
    AbstractByteParser(port::ByteBuffer* buffer);
    virtual void setBuffer(port::ByteBuffer* buffer);
    virtual port::ByteBuffer* getBuffer();
};


//complete object
//reads only type descriptor and redirects to certain ParserProcessor
class UnitTestLocalByteParser:
        public AbstractByteParser,
        public ProgramObject
{
private:
    DataPackage       result_;
    bool              package_ready_;
public:
   UnitTestLocalByteParser(port::ByteBuffer* buffer);
public:
   virtual void parseCommand(Code cmd) override;
   virtual bool packageReady() const override;
   virtual DataPackage getParsedPackage() override;
   virtual bool isEmptyResult() const override;
public:
   virtual void parseData();
   virtual void checkCode(Code cmd);
};


///idea

class ParserComponent
{
protected:
    ParserComponent*                  parent_;
    port::ByteBuffer*                 buffer_;
    QSharedPointer<ParsedDataPackage> package_;
    static TypeDescriptor             current_type_;
public:
    virtual ParserComponent* getParent()
    {
        return  this->parent_;
    }
    virtual void setParent(ParserComponent* newParent)
    {
        this->parent_ = newParent;
    }
    virtual void addChild(QSharedPointer<ParserComponent> child){};
    virtual bool isComposite() const
    {
        return false;
    }
    virtual bool parseCommand(QSharedPointer<ParsedDataPackage>& result) = 0;//if true -> still parsing
    void setBuffer(port::ByteBuffer* newBuffer)
    {
        buffer_ = newBuffer;
    }
};

class ComplexParser:
        public ParserComponent,
        public ProgramObject
{
protected:
    QList<QSharedPointer<ParserComponent>> children_;
    Code                                   commands_count_;
public:
    ComplexParser(Code commands_count):
        commands_count_{commands_count}{}
    virtual bool isComposite() const override
    {
        return true;
    }
    virtual void addChild(QSharedPointer<ParserComponent> child) override
    {
        throwIf(child.isNull() , "Child cannot be a nullptr!");

        child->setParent(this);
        child->setBuffer(buffer_);

        children_.push_back(child);
    }
    void checkCode(Code cmd, std::string class_name)
    {
        throwIf(cmd >= commands_count_ , { "Command error : " +
                                           class_name +
                                           " , out of range"});
    }
};

///real

class GlobalParser:
        public ComplexParser
{
public:
    GlobalParser():
        ComplexParser{GlobalCommand::GLOBAL_COMMAND_COUNT}{}
    virtual bool parseCommand(QSharedPointer<ParsedDataPackage>& result) override
    {
        package_ = QSharedPointer<ParsedDataPackage>::create();


        Code cmd {buffer_->getByte()};
        checkCode(cmd, typeid (*this).name() );

        while (children_.at(cmd)->parseCommand(package_))//make a map further
        {
            cmd = buffer_->getByte();
            checkCode(cmd, typeid (*this).name() );
        }

        result = std::move(package_);

        return false;
    }
};

class GlobalStartParser:
        public ParserComponent
{
public:
    virtual bool parseCommand(QSharedPointer<ParsedDataPackage>& result) override
    {
        return true;
    }
};

using EmptyParser = GlobalStartParser;

class EndParser:
        public ParserComponent
{
public:
    virtual bool parseCommand(QSharedPointer<ParsedDataPackage>& result) override
    {
        return false;
    }
};

class TestCaseParser:
        public ComplexParser
{
public:
    TestCaseParser():
        ComplexParser{TestCaseCommand::TEST_CASE_COMMAND_COUNT }{}
    virtual bool parseCommand(QSharedPointer<ParsedDataPackage>& result) override//test obj
    {
        package_ = QSharedPointer<ParsedDataPackage>::create();//create a test case
        result->addChild(package_);//append a test case

        QByteArray name_bytes;

        char byte {buffer_->getByte()};
        while(byte != '\0')
        {
            name_bytes.append(byte);
            byte = buffer_->getByte();
        }

        package_->setBytes(name_bytes);

        Code cmd {buffer_->getByte()};
        checkCode(cmd, typeid (*this).name() );

        while (children_.at(cmd)->parseCommand(package_))//make a map further
        {
            cmd = buffer_->getByte();
            checkCode(cmd, typeid (*this).name() );
        }

        result = package_;

        return true;
    }
};

class UnitTestParser:
        public ComplexParser
{
public:
    UnitTestParser():
        ComplexParser{UnitTestCommand::COMMANDS_COUNT}{}
    virtual bool parseCommand(QSharedPointer<ParsedDataPackage>& result) override//test obj
    {
        package_ = QSharedPointer<ParsedDataPackage>::create();//create a unit test
        result->addChild(package_);//append a unit test

        Code cmd {buffer_->getByte()};
        checkCode(cmd, typeid (*this).name() );

        while (children_.at(cmd)->parseCommand(package_))//make a map further
        {
            cmd = buffer_->getByte();
            checkCode(cmd, typeid (*this).name() );
        }

        result = package_;

        return true;
    }
};

class NameParser:
        public ParserComponent
{
public:
    virtual bool parseCommand(QSharedPointer<ParsedDataPackage>& result) override//unit test
    {
        package_ = QSharedPointer<ParsedDataPackage>::create();//create a name
        result->addChild(package_);//append a name
        package_->setIsLeaf(true);

        QByteArray name_bytes;

        char byte {buffer_->getByte()};
        while(byte != '\0')
        {
            name_bytes.append(byte);
            byte = buffer_->getByte();
        }

        package_->setBytes(name_bytes);

        return true;
    }
};

class TypeDescriptorParser:
        public ParserComponent
{
public:
    virtual bool parseCommand(QSharedPointer<ParsedDataPackage>& result) override//unit test
    {
        package_ = QSharedPointer<ParsedDataPackage>::create();//create a type desc
        result->addChild(package_);//append a type desc
        package_->setIsLeaf(true);

        QByteArray desc_result;

        desc_result.append(buffer_->getByte());

        current_type_ =  static_cast<TypeDescriptor>((uint8_t)desc_result[0]);

        package_->setBytes(desc_result);

        return true;
    }
};

class ValueDescriptorParser:
        public ParserComponent
{
public:
    virtual bool parseCommand(QSharedPointer<ParsedDataPackage>& result) override//unit test
    {
        package_ = QSharedPointer<ParsedDataPackage>::create();//create a type desc
        result->addChild(package_);//append a type desc
        package_->setIsLeaf(true);

        QByteArray value_result;

        for(int i{0}; i < TypesSizes::getSize(current_type_) ; ++i)
                value_result.append(buffer_->getByte());

        package_->setBytes(value_result);

        return true;
    }
};

class TestResultParser:
        public ParserComponent
{
public:
    virtual bool parseCommand(QSharedPointer<ParsedDataPackage>& result) override//unit test
    {
        package_ = QSharedPointer<ParsedDataPackage>::create();//create a type desc
        result->addChild(package_);//append a type desc
        package_->setIsLeaf(true);

        QByteArray test_result;

        test_result.append(buffer_->getByte());

        package_->setBytes(test_result);

        return true;
    }
};

}

