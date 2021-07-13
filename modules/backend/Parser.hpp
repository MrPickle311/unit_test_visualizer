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

enum GlobalCommand : uint8_t{ START					   = 0 ,
                              SENDING_TEST_CASE        = 1 ,
                              END_ENTIRE_TRANSACTION   = 4 ,
                              CMD_CNT};

enum TestCaseCommand : uint8_t{ SENDING_UNIT_TEST_RESULT = 2 ,
                                END_SENDING_TEST_CASE    = 3};

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

class ParserDataPackage
{
private:
    QList<QSharedPointer<ParserDataPackage>> children;
    bool is_leaf_;
    QByteArray bytes_;//if its composite , bytes_ is empty
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

class TestCaseParser:
        public AbstractByteParser
{

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

}

