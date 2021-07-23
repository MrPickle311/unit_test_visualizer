#pragma once

#include "PortSettings.hpp"
#include <QEventLoop>

namespace port
{

class ByteBufferInterface:
        public QObject
{
    Q_OBJECT;
public:
    virtual ~ByteBufferInterface(){}
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

class ByteBuffer :
        public ByteBufferInterface,
        public ProgramObject
{
    Q_OBJECT;
private:
    QByteArray received_bytes_;
    QMutex     data_mutex_;
    QEventLoop loop_;
private:
    QByteArray splitByteArray(size_t position);
    void       waitForData();
public:
    ByteBuffer(QObject *parent = nullptr);
    virtual ~ByteBuffer(){}
    void       appendBytes(const QByteArray& array);
    void       appendByte(char byte);
    QByteArray getAllBytes() noexcept;
    QByteArray getBytes(size_t count);
    char       getByte();
    bool       isEmpty() const;
    size_t     size()    const;
};

//one operator per one port
class PortStateOperator :
        public QObject,//it only opens a port , nothing else
        public ProgramObject
{
    Q_OBJECT;
protected:
    QSerialPort     current_port_;
    QSerialPortInfo current_port_info_;
public:
    PortStateOperator(QObject* parent = nullptr);
    PortStateOperator(PortFlowSettings settings,
                      QSerialPortInfo port ,
                      QObject* parent = nullptr);
    virtual ~PortStateOperator(){}
public slots:
    void changePort(QSerialPortInfo port);
    void changeSettings(PortFlowSettings settings);//only copy ,so nothing unexpected will happen
    void closePort();
    bool openPort();
    bool isOpen() const;
    QSerialPort::SerialPortError getError() const;
};

class PortFlowOperator:
        public PortStateOperator
{
    Q_OBJECT;
private:
    void makeConnections();
public:
    PortFlowOperator( QObject* parent = nullptr);
    PortFlowOperator( PortFlowSettings settings ,
                      QSerialPortInfo  port     ,
                      QObject* parent = nullptr);
    virtual ~PortFlowOperator(){}
public:
    void sendBytesToPort(const QByteArray& array);
    QByteArray getAllBytesFromPort();
signals:
    void dataArrived();//external signal ,for user , data <- serial
    void dataSent();//external signal ,for user data -> serial
};

class BufferedPortFlowOperator:
        public PortFlowOperator
{
    Q_OBJECT;
protected:
    ByteBuffer* input_byte_buffer_;
    ByteBuffer* output_byte_buffer_;
private:
    void makeInputBufferConnections();
    void makeOutputBufferConnections();
    void checkBuffer(ByteBuffer* buffer) noexcept(false);
public:
    BufferedPortFlowOperator(QObject* parent = nullptr);
    BufferedPortFlowOperator( PortFlowSettings settings ,
                              QSerialPortInfo  port     ,
                              QObject* parent = nullptr);
    virtual ~BufferedPortFlowOperator(){}
protected slots:
    void sendDataFromPortToBuffer(); //invoked automatically
    void sendDataFromBufferToPort(); //invoked automatically
public:
    void setInputByteBuffer(ByteBuffer* byte_buffer);
    void setOutputByteBuffer(ByteBuffer* byte_buffer);
};

}
