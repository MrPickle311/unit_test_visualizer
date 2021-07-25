#pragma once

#include "PortSettings.hpp"
#include <QEventLoop>
#include "Interfaces.hpp"

namespace backend
{

class ByteBuffer :
        public interface::ByteBuffer,
        public global::ProgramObject
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
    QByteArray getAllBytes()                        noexcept;
    QByteArray getBytes(size_t count);
    char       getByte();
    bool       isEmpty()                            const;
    size_t     size()                               const;
};

//one operator per one port
class PortStateOperator :
        public QObject,//it only opens a port , nothing else
        public global::ProgramObject
{
    Q_OBJECT;
protected:
    QSerialPort     current_port_;
    QSerialPortInfo current_port_info_;
private:
    void makeConnections();
public:
    PortStateOperator(QObject* parent = nullptr);
    PortStateOperator(PortFlowSettings settings,
                      QSerialPortInfo port ,
                      QObject* parent = nullptr);
    virtual ~PortStateOperator(){}
public slots:
    void    changePort(QSerialPortInfo port);
    void    changeSettings(PortFlowSettings settings);//only copy ,so nothing unexpected will happen
    void    closePort();
    bool    openPort();
    bool    isOpen()                                    const;
    QString getError()                                  const;
protected slots:
    void deviceErrorService(QSerialPort::SerialPortError error);
signals:
    void deviceErrorOccurred(QString what);
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
    void       sendBytesToPort(const QByteArray& array);
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
