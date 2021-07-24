#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
#include "PortSettings.hpp"
#include "ParsedDataTypes.hpp"
#include "ConvertedDataTypes.hpp"
#include <variant>

using AcceptedTypes = std::variant< QSharedPointer<backend::UnitTestDataPackage> ,
                                    QSharedPointer<backend::TestCaseDataPackage> ,
                                    QSharedPointer<backend::TransactionDataPackage> ,
                                    std::monostate >;

namespace interface
{

class PortScanner
{
public:
    virtual ~PortScanner(){}
    virtual QSerialPortInfo getPortByNumber(uint port_nmbr) const = 0;
    virtual QSerialPortInfo getPortByName(QString port_name) const = 0;
    virtual QList<int>      getProductIndetifiers() const = 0;
    virtual QStringList     getPortNames() const = 0;
    virtual QStringList     getPortDescriptions() const = 0;
    virtual QStringList     getCompletePortData() const = 0;
    virtual void            rescan() = 0;
};

class ByteBuffer:
        public QObject
{
    Q_OBJECT;
public:
    virtual ~ByteBuffer(){}
    virtual void       appendBytes(const QByteArray& array) = 0;
    virtual void       appendByte(char byte) = 0;
    virtual QByteArray getAllBytes() noexcept = 0;
    virtual QByteArray getBytes(size_t count) = 0;
    virtual char       getByte() = 0;
    virtual bool       isEmpty() const = 0;
    virtual size_t     size()    const = 0;
signals:
   void bytesArrived(size_t count);
   void bytesExtracted(size_t count);
};

//class PortStateOperator :
//        public QObject
//{
//    Q_OBJECT;
//public:
//    virtual ~PortStateOperator(){}
//public slots:
//    virtual void changePort(QSerialPortInfo port)  = 0;
//    virtual void changeSettings(backend::PortFlowSettings settings)  = 0;
//    virtual void closePort()  = 0;
//    virtual bool openPort()  = 0;
//    virtual bool isOpen() const  = 0;
//    virtual QSerialPort::SerialPortError getError() const = 0;
//};
//
//class PortFlowOperator:
//        public PortStateOperator
//{
//    Q_OBJECT;
//public:
//    virtual ~PortFlowOperator(){}
//    virtual void sendBytesToPort(const QByteArray& array) = 0;
//    virtual QByteArray getAllBytesFromPort() = 0;
//signals:
//    void dataArrived();
//    void dataSent();
//};
//
//class BufferedPortFlowOperator:
//        public PortFlowOperator
//{
//    Q_OBJECT;
//public:
//    virtual ~BufferedPortFlowOperator(){}
//protected slots:
//    virtual void sendDataFromPortToBuffer() = 0;
//    virtual void sendDataFromBufferToPort() = 0;
//public:
//    virtual void setInputByteBuffer(ByteBuffer* byte_buffer) = 0;
//    virtual void setOutputByteBuffer(ByteBuffer* byte_buffer) = 0;
//};



class ParserComponent
{
public:
    virtual ParserComponent* getParent() = 0;
    virtual void setParent(ParserComponent* newParent) = 0;
    virtual void addChild(uint8_t cmd , QSharedPointer<ParserComponent> child) = 0;
    virtual bool isComposite() const = 0;
    virtual void setBuffer(interface::ByteBuffer* newBuffer) = 0;

    //default as hooks
    virtual void       createPackage(){} ;
    virtual QSharedPointer<backend::TransactionDataPackage> getPackage(){};
    virtual void               startProcessing(){};

    virtual bool parseCommand(AcceptedTypes result) = 0;//if true -> still parsing
};

class Converter
{
protected:
    const backend::TransactionDataPackage& pack_;
public:
    Converter(const backend::TransactionDataPackage& pack):
        pack_{pack}{}
    virtual backend::Transaction getConvertedTransaction() = 0;
    virtual void reset() = 0;
};

}
