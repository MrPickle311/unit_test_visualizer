#pragma once

#include <QByteArray>
#include <QStateMachine>
#include <QState>
#include <QSignalTransition>
#include "PortOperator.hpp"
#include <memory>

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

enum GlobalCommand{ START					 = 0 ,
                    SENDING_TEST_CASE        = 1 ,
                    SENDING_UNIT_TEST_RESULT = 2 ,
                    END_SENDING_TEST_CASE    = 3 ,
                    END_ENTIRE_TRANSACTION   = 4 };

//enum TestCaseCommand{
//    SENDING_UNIT_TEST_RESULT = 2 ,
//    END_SENDING_TEST_CASE    = 3
//};

enum UnitTestCommand{ SENDING_TYPE_DESCRIPTOR 		= 0 ,
                      SENDING_NAME					= 1 ,
                      SENDING_CURRENT_VALUE			= 2 ,
                      SENDING_EXPECTED_VALUE		= 3 ,
                      SENDING_TEST_RESULT			= 4 ,
                      SENDING_LINE_NMBR				= 5 ,
                      SENDING_LOWER_VALUE           = 6 ,
                      SENDING_UPPER_VALUE           = 7 ,
                      END_SENDING_UNIT_TEST_RESULT	= 8 ,
                      COMMANDS_COUNT};

enum TypeDescriptor{ UINT8_T  = 0  ,
                     UINT16_T = 1  ,
                     UINT32_T = 2  ,
                     UINT64_T = 3  ,

                     INT8_T   = 4  ,
                     INT16_T  = 5  ,
                     INT32_T  = 6  ,
                     INT64_T  = 7  ,

                     BOOL     = 8  ,
                     CHAR     = 9  ,
                     PTR	  = 10 ,
                     BIT	  = 11 ,
                     TYPES_COUNT};


struct Command
{
    Code command_code_;
};

struct DataPackage
{
    QByteArrayList parsed_data_;
};

struct UnitTestPackage:
        public DataPackage
{
    QByteArray unit_test_name_;
    Code       type_descriptor_;
};

struct ProcessorDependencies
{
    ProcessorDependencies();
    ProcessorDependencies(QSharedPointer<QByteArray>  result ,port::ByteBuffer* buffer);
    QSharedPointer<QByteArray>  result_;
    port::ByteBuffer*           buffer_;
};

class AbstractProcessor
{
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

class ExpectedValueProcessor:
        public Processor
{
public:
    virtual QByteArray process(port::ByteBuffer* buffer) override;
};

class CurrentValueProcessor:
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

class Processors
{
private:
    static QMutex mutex_;
    static QMap<UnitTestCommand,std::shared_ptr<Processor>> processors_;
    static bool processors_initialized_;
private:
    template<typename T>
    static void addProcessor(UnitTestCommand code);
    static void initProcessors();
protected:
    Processors();
public:
    static Processor* getProcessor(UnitTestCommand cmd);
};


//interface for every kind of parser
//pure abstraction , no data
class AbstractParser //:
        //public QObject
{
   // Q_OBJECT;
private :
    virtual void parseCommand(Command* cmd) = 0;
public:
    virtual bool packageReady() = 0;
    virtual DataPackage getParsedPackage() = 0;
    virtual bool atStart() = 0;
//signals:
//    void packageParsed();
};

//abstraction for a byte parser
class AbstractLocalByteParser:
        public AbstractParser
{
public:
    virtual void setBuffer(port::ByteBuffer* buffer) = 0;
    virtual port::ByteBuffer* getBuffer() = 0;
};


//incoming data -> DataPackage
//real object but not complete yet
//class LocalByteParser:
//        public AbstractLocalByteParser
//{
//    Q_OBJECT;
//protected:
//    port::ByteBuffer* buffer_;
//    DataPackage       result_;
//
//    // AbstractParser interface
//public:
//    virtual void parseCommand(Command* cmd);
//    virtual bool packageReady() override;
//    virtual DataPackage getParsedPackage() override;
//    virtual bool atStart() override;
//
//    // AbstractLocalByteParser interface
//public:
//    virtual void setBuffer(port::ByteBuffer* buffer) override;
//    virtual port::ByteBuffer* getBuffer() override;
//};
//

//complete object
//reads only type descriptor and redirects to certain ParserProcessor
class UnitTestLocalByteParser:
        public AbstractLocalByteParser,
        public ProgramObject
{
private:
   //LocalByteParser byte_parser_;
    port::ByteBuffer* buffer_;
    DataPackage       result_;
    bool              package_ready_;
    // AbstractParser interface
public:
    UnitTestLocalByteParser(port::ByteBuffer* buffer);

   virtual void parseCommand(Command* cmd);
   virtual bool packageReady() override;
   virtual DataPackage getParsedPackage() override;
   virtual bool atStart() override;

   // AbstractLocalByteParser interface
public:
   virtual void setBuffer(port::ByteBuffer* buffer) override;
   virtual port::ByteBuffer* getBuffer() override;

public:
   void parseData();
   void checkCode(Code cmd);
};

}

