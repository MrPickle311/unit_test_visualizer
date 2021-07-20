#pragma once

#include "PortSettings.hpp"

namespace port
{

class ByteBuffer :
        public QObject,
        public ProgramObject
{
    Q_OBJECT;
private:
    QByteArray received_bytes_;
    QMutex     data_mutex_;
private:
    QByteArray splitByteArray(size_t position);
public:
    ByteBuffer(QObject *parent = nullptr);
    void       appendBytes(const QByteArray& array);
    void       appendByte(char byte);
    QByteArray getAllBytes() noexcept;
    QByteArray getBytes(size_t count);
    char       getByte();
    bool       isEmpty() const;
    size_t     size()    const;
signals:
   void bytesArrived(size_t count);
   void bytesExtracted(size_t count);
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
public slots:
    void changePort(QSerialPortInfo port);
    void changeSettings(PortFlowSettings settings);//only copy ,so nothing unexpected will happen
    void closePort();
    bool openPort();
    bool isOpen() const;
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
protected slots:
    void sendDataFromPortToBuffer(); //invoked automatically
    void sendDataFromBufferToPort(); //invoked automatically
public:
    void setInputByteBuffer(ByteBuffer* byte_buffer);
    void setOutputByteBuffer(ByteBuffer* byte_buffer);
};

}
