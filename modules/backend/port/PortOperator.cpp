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

    throwIf(static_cast<int>(count) > received_bytes_.size(),
            "requested count of bytes > received_bytes_.size()!\n");

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

PortStateOperator::PortStateOperator(QObject* parent):
    QObject{parent},
    current_port_{this},
    current_port_info_{}
{}

PortStateOperator::PortStateOperator(QSerialPortInfo port ,
                                     PortFlowSettings settings,
                                     QObject* parent):
    QObject{parent},
    current_port_{this},
    current_port_info_{port}
{
    changeSettings(settings);
}

void PortStateOperator::changePort(QSerialPortInfo port)
{
    current_port_info_ = port;
    current_port_.setPort(current_port_info_);
}

void PortStateOperator::changeSettings(PortFlowSettings settings)
{
    current_port_.setBaudRate(settings.baudRate());
    current_port_.setDataBits(settings.dataBits());
    current_port_.setParity(settings.parity());
    current_port_.setFlowControl(settings.flowControl());
    current_port_.setStopBits(settings.stopBits());
}

void PortStateOperator::closePort()
{
    if(current_port_.isOpen())
        current_port_.close();
}

bool PortStateOperator::openPort()
{
    throwIf(current_port_.isOpen(), {"This port("  +
            current_port_.portName().toStdString() + " ) is arleady open!"});

    return current_port_.open(QSerialPort::ReadWrite);
}

void PortFlowOperator::makeConnections()
{
    connect(&current_port_ , &QSerialPort::readyRead ,
            this , &PortFlowOperator::dataArrived );

    connect(&current_port_ , &QSerialPort::bytesWritten ,
            this , &PortFlowOperator::dataSent );
}



/*

BufferedPortFlowOperator::BufferedPortFlowOperator(QObject* parent):
    current_byte_buffer_{nullptr}
{
     current_byte_buffer_->setParent(parent);
}

BufferedPortFlowOperator::BufferedPortFlowOperator(ByteBuffer* data_handler ,
                                           QObject*    parent):
    current_byte_buffer_{data_handler}
{
    current_byte_buffer_->setParent(parent);
}
*/

void BufferedPortFlowOperator::setOutputByteBuffer(ByteBuffer* byte_buffer)
{
    output_byte_buffer_ = byte_buffer;
}

/*
PortFlowOperator::PortFlowOperator(QObject* parent):
    BufferedPortFlowOperator{QSerialPort::ReadOnly , parent}
{
    makeConnections();
}
*/

void PortFlowOperator::sendBytesToPort(const QByteArray& array)
{
    current_port_.write(array);
}

QByteArray PortFlowOperator::getAllBytesFromPort()
{
    return current_port_.readAll();
}

/*
PortFlowOperator::PortFlowOperator(PortFlowSettings settings ,
                                     QSerialPortInfo  port     ,
                                     ByteBuffer* byte_buffer   ,
                                     QObject* parent ):
    BufferedPortFlowOperator{settings,
                         port ,
                         QSerialPort::ReadOnly ,
                         byte_buffer ,
                         parent }
{
    makeConnections();
}

*/

void BufferedPortFlowOperator::sendDataFromPortToBuffer()
{
    throwIf(input_byte_buffer_ == nullptr , "Byte buffer not initialized!");

    input_byte_buffer_->appendBytes(std::move(current_port_.readAll()));
    emit dataArrived();
}

void BufferedPortFlowOperator::makeConnections()
{
    connect(input_byte_buffer_ , &ByteBuffer::bytesArrived ,
            this , &BufferedPortFlowOperator::sendDataFromBufferToPort );
}

void BufferedPortFlowOperator::sendDataFromBufferToPort()
{
    throwIf(output_byte_buffer_ == nullptr , "Byte buffer not initialized!");

    if(current_port_.isWritable())
        current_port_.write(output_byte_buffer_->getAllBytes());
}


}
