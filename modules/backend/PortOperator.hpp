#pragma once

#include "PortSettings.hpp"

namespace port
{

class ByteBuffer :
        public QObject
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
    QByteArray getAllBytes() noexcept;
    QByteArray getBytes(size_t count);
    bool       isEmpty() const;
    size_t     size()    const;
signals:
   void bytesArrived(size_t count);
   void bytesExtracted(size_t count);
};

//one operator per one port
class PortOperator :
        public QObject//it only opens a port , nothing else
{
    Q_OBJECT;
protected:
    QSerialPort           current_port_;
    QSerialPortInfo       current_port_info_;
    QSerialPort::OpenMode open_mode_;
protected:
    virtual void openHook(){};
    virtual void closeHook(){};
public:
    PortOperator(QSerialPort::OpenMode open_mode, QObject* parent);
public slots:
    void changePort(QSerialPortInfo port);
    void changeSettings(PortFlowSettings settings);//only copy ,so nothing unexpected will happen
    void closePort();
    bool openPort();
};

class BufferedPortOperator:
        public PortOperator
{
    Q_OBJECT;
protected:
    ByteBuffer* current_byte_buffer_;
public:
    void setByteBuffer(ByteBuffer* handler);
    BufferedPortOperator(QSerialPort::OpenMode  open_mode ,
                         QObject* parent = nullptr);
    BufferedPortOperator(PortFlowSettings      settings  ,
                         QSerialPortInfo       port      ,
                         QSerialPort::OpenMode open_mode ,
                         ByteBuffer* byte_buffer         ,
                         QObject* parent = nullptr);
};

class PortInputOperator :
        public BufferedPortOperator
{
    Q_OBJECT;
private:
    void makeConnections();
public:
    PortInputOperator(QObject* parent = nullptr);
    PortInputOperator(PortFlowSettings settings ,
                      QSerialPortInfo  port     ,
                      ByteBuffer* byte_buffer ,
                      QObject* parent = nullptr);
public slots:
    void sendDataFromPortToBuffer(); //invoked automatically
signals:
    void dataArrived();//external signal ,for user , data <- serial
};

/*
class PortOutputOperator:
        public PortOperator
{
    Q_OBJECT;
private:
    ByteBuffer* current_data_handler_;
public:
    void sendDataFromBufferToPort();
signals:
    void dataSent();//external signal ,for user data -> serial
};
*/

}
