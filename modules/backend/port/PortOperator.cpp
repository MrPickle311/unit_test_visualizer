#include "../PortOperator.hpp"
#include <QDebug>

namespace port
{

//data handler

void ByteBuffer::appendBytes(const QByteArray& array)
{
    QMutexLocker lock{&data_mutex_};
    received_bytes_.append(array);
    emit bytesArrived(array.size());
}

QByteArray ByteBuffer::splitByteArray(size_t count)
{
    QMutexLocker lock{&data_mutex_};

    if(static_cast<int>(count) > received_bytes_.size())
        throw std::logic_error{"requested count of bytes > received_bytes_.size()!\n"};

    QByteArray temp {received_bytes_.left(count)};
    received_bytes_ = received_bytes_.right( received_bytes_.size() - count );

    emit bytesExtracted(count );
    return temp;
}

ByteBuffer::ByteBuffer(QObject *parent) :
    QObject{parent},
    received_bytes_{}
{}

QByteArray ByteBuffer::getAllBytes() noexcept
{
    if(this->isEmpty())
        return QByteArray{};
    return getBytes(received_bytes_.size());
}

QByteArray ByteBuffer::getBytes(size_t count)
{
    return splitByteArray(count);
}

bool ByteBuffer::isEmpty() const
{
    return received_bytes_.isEmpty();
}

size_t ByteBuffer::size() const
{
    return received_bytes_.size();
}

PortOperator::PortOperator(QSerialPort::OpenMode open_mode , QObject* parent):
    QObject{parent},
    current_port_{this},
    current_port_info_{},
    open_mode_{open_mode}
{}

void PortOperator::changePort(QSerialPortInfo port)
{
    current_port_info_ = port;
    current_port_.setPort(current_port_info_);
}

void PortOperator::changeSettings(PortFlowSettings settings)
{
    current_port_.setBaudRate(settings.baudRate());
    current_port_.setDataBits(settings.dataBits());
    current_port_.setParity(settings.parity());
    current_port_.setFlowControl(settings.flowControl());
    current_port_.setStopBits(settings.stopBits());
}

void PortOperator::closePort()
{
    if(current_port_.isOpen())
        current_port_.close();
    closeHook();
}

bool PortOperator::openPort()
{
    if(current_port_.isOpen())
        throw std::logic_error{std::string{"This port(" } +
                               current_port_.portName().toStdString() +
                                       std::string{" ) is arleady open!"}};
    openHook();
    return current_port_.open(this->open_mode_);
}

BufferedPortOperator::BufferedPortOperator(QIODevice::OpenMode open_mode,
                                           QObject* parent):
    PortOperator{open_mode, parent},
    current_byte_buffer_{nullptr}
{}

BufferedPortOperator::BufferedPortOperator(PortFlowSettings       settings     ,
                                           QSerialPortInfo        port         ,
                                           QSerialPort::OpenMode  open_mode    ,
                                           ByteBuffer*            data_handler ,
                                           QObject*               parent):
    PortOperator{open_mode , parent},
    current_byte_buffer_{data_handler}
{
    changePort(port);
    changeSettings(settings);
}

void BufferedPortOperator::setByteBuffer(ByteBuffer* handler)
{
    current_byte_buffer_ = handler;
}

void PortInputOperator::makeConnections()
{
    connect(&current_port_ , &QSerialPort::readyRead ,
            this , &PortInputOperator::sendDataFromPortToBuffer );
}

PortInputOperator::PortInputOperator(QObject* parent):
    BufferedPortOperator{QSerialPort::ReadOnly , parent}
{
    makeConnections();
}

PortInputOperator::PortInputOperator(PortFlowSettings settings ,
                                     QSerialPortInfo  port     ,
                                     ByteBuffer* byte_buffer   ,
                                     QObject* parent ):
    BufferedPortOperator{settings,
                         port ,
                         QSerialPort::ReadOnly ,
                         byte_buffer ,
                         parent }
{
    makeConnections();
}

void PortInputOperator::sendDataFromPortToBuffer()
{
    //TODO: logic error if handler_ == nullptr!!!
    current_byte_buffer_->appendBytes(std::move(current_port_.readAll()));
    emit dataArrived();
}

void PortOutputOperator::makeConnections()
{
    connect(current_byte_buffer_ , &ByteBuffer::bytesArrived ,
            this , &PortOutputOperator::sendDataFromBufferToPort );
}

PortOutputOperator::PortOutputOperator(QObject* parent):
    BufferedPortOperator{QSerialPort::WriteOnly , parent}
{
    makeConnections();
}

PortOutputOperator::PortOutputOperator(PortFlowSettings settings ,
                                       QSerialPortInfo  port     ,
                                       ByteBuffer* byte_buffer   ,
                                       QObject* parent ):
      BufferedPortOperator{settings,
                           port ,
                           QSerialPort::WriteOnly ,
                           byte_buffer ,
                           parent }
{
    makeConnections();
}

void PortOutputOperator::sendDataFromBufferToPort()
{
    if(current_port_.isWritable())
        current_port_.write(current_byte_buffer_->getAllBytes());
}


}
