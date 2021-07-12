#pragma once

#include <QByteArray>
#include <QStateMachine>
#include <QState>
#include <QSignalTransition>
#include "PortOperator.hpp"

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

enum TestCaseCommand{ SENDING_TYPE_DESCRIPTOR 		= 0 ,
                      SENDING_NAME					= 1 ,
                      SENDING_CURRENT_VALUE			= 2 ,
                      SENDING_EXPECTED_VALUE		= 3 ,
                      SENDING_TEST_RESULT			= 4 ,
                      SENDING_LINE_NMBR				= 5 ,
                      SENDING_LOWER_VALUE           = 6 ,
                      SENDING_UPPER_VALUE           = 7 ,
                      END_SENDING_UNIT_TEST_RESULT	= 8 };

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

class AbstractProcessor:
        public QObject
{
    Q_OBJECT;
public:
    virtual ~AbstractProcessor(){}
    virtual QByteArray process() = 0;
};

class Processor:
        public AbstractProcessor,
        public ProgramObject
{
    Q_OBJECT;
protected:
    port::ByteBuffer* buffer_;
public:
    Processor(port::ByteBuffer* buffer_ = nullptr);
public:
    void setBuffer(port::ByteBuffer* newBuffer);
};

class TypeDescriptorProcessor:
        public Processor
{
public:
    virtual QByteArray process() override;
};

class NameProcessor:
        public Processor
{
public:
    virtual QByteArray process() override;
};

class ValueProcessor:
        public Processor
{
public:
    virtual QByteArray process() override;
};


//interface for every kind of parser
//pure abstraction , no data
class AbstractParser :
        public QObject
{
    Q_OBJECT;
private slots:
    virtual void parseCommand(Command* cmd) = 0;
public:
    virtual bool packageReady() = 0;
    virtual DataPackage getParsedPackage() = 0;
    virtual bool atStart() = 0;
signals:
    void packageParsed();
};

//abstraction for a byte parser
class AbstractLocalByteParser:
        public AbstractParser
{
    Q_OBJECT;
public:
    virtual void setBuffer(port::ByteBuffer* buffer) = 0;
    virtual port::ByteBuffer* getBuffer() = 0;
};


//incoming data -> DataPackage
//real object but not complete yet
class LocalByteParser:
        public AbstractLocalByteParser
{
    Q_OBJECT;
private:
    port::ByteBuffer* buffer_;
    QSharedPointer<DataPackage> result_;

    // AbstractParser interface
public:
    virtual void parseCommand(Command* cmd);
    virtual bool packageReady() override;
    virtual DataPackage getParsedPackage() override;
    virtual bool atStart() override;

    // AbstractLocalByteParser interface
public:
    virtual void setBuffer(port::ByteBuffer* buffer) override;
    virtual port::ByteBuffer* getBuffer() override;
};


//complete object
//reads only type descriptor and redirects to certain ParserProcessor
class UnitTestLocalByteParser:
        public AbstractLocalByteParser
{
    Q_OBJECT;
private:
   LocalByteParser byte_parser_;
   QStateMachine machine;

   // AbstractParser interface
public:
   virtual void parseCommand(Command* cmd);
   virtual bool packageReady() override;
   virtual DataPackage getParsedPackage() override;
   virtual bool atStart() override;

   // AbstractLocalByteParser interface
public:
   virtual void setBuffer(port::ByteBuffer* buffer) override;
   virtual port::ByteBuffer* getBuffer() override;
};

}

