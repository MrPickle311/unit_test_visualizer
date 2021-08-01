#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
#include "PortSettings.hpp"
#include "ParsedDataTypes.hpp"
#include "ConvertedDataTypes.hpp"
#include <variant>

namespace interface
{

/// PortScanner common interface
class PortScanner
{
public:
    virtual ~PortScanner(){}
    virtual QSerialPortInfo getPortByNumber(uint port_nmbr)  const = 0;
    virtual QSerialPortInfo getPortByName(QString port_name) const = 0;
    virtual QList<int>      getProductIndetifiers()          const = 0;
    virtual QStringList     getPortNames()                   const = 0;
    virtual QStringList     getPortDescriptions()            const = 0;
    virtual QStringList     getCompletePortData()            const = 0;
    virtual void            rescan()                               = 0;
};

/// ByteBuffer common interface
class ByteBuffer:
        public QObject
{
    Q_OBJECT;
public:
    virtual ~ByteBuffer(){}
    virtual void       appendBytes(const QByteArray& array)           = 0;
    virtual void       appendByte(char byte)                          = 0;
    virtual QByteArray getAllBytes()                        noexcept  = 0;
    virtual QByteArray getBytes(size_t count)                         = 0;
    virtual char       getByte()                                      = 0;
    virtual bool       isEmpty()                            const     = 0;
    virtual size_t     size()                               const     = 0;
signals:
   void bytesArrived(size_t count);
   void bytesExtracted(size_t count);
};

/// Types that can be passed to ParserComponent::parseCommand()
/// and be recognized in this method
using AcceptedTypes = std::variant< QSharedPointer<backend::UnitTestDataPackage> ,
                                    QSharedPointer<backend::TestCaseDataPackage> ,
                                    QSharedPointer<backend::TransactionDataPackage> ,
                                    std::monostate >;

/// ParserComponent common interface
class ParserComponent
{
public:
    virtual ParserComponent* getParent()                                               = 0;
    virtual void setParent(ParserComponent* newParent)                                 = 0;
    virtual void addChild(uint8_t cmd , QSharedPointer<ParserComponent> child)         = 0;
    virtual bool isComposite()                                                  const  = 0;
    virtual void setBuffer(interface::ByteBuffer* newBuffer)                           = 0;
    virtual bool parseCommand(AcceptedTypes result)                                    = 0;//if true -> still parsing

    //by default as hooks
    virtual void                                            createPackage()  {};
    virtual QSharedPointer<backend::TransactionDataPackage> getPackage()     {return {};};
    virtual void                                            startProcessing(){};
};

/// Converter common interface
class Converter
{
public:
    virtual void                 setPack(backend::TransactionDataPackage const * pack) = 0;
    virtual backend::Transaction getConvertedTransaction()                             = 0;
    virtual void                 reset()                                               = 0;
};

}
