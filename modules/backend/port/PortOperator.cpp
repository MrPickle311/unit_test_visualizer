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

PortStateOperator::PortStateOperator(PortFlowSettings settings,
                                     QSerialPortInfo port ,
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

///

PortFlowOperator::PortFlowOperator(QObject* parent):
    PortStateOperator{parent}
{
    makeConnections();
}

PortFlowOperator::PortFlowOperator( PortFlowSettings settings,
                                    QSerialPortInfo port,
                                    QObject* parent):
    PortStateOperator{settings , port , parent}
{
    makeConnections();
}

void PortFlowOperator::makeConnections()
{
    connect(&current_port_ , &QSerialPort::readyRead ,
            this , &PortFlowOperator::dataArrived );

    connect(&current_port_ , &QSerialPort::bytesWritten ,
            this , &PortFlowOperator::dataSent );
}

void PortFlowOperator::sendBytesToPort(const QByteArray& array)
{
    current_port_.write(array);
}

QByteArray PortFlowOperator::getAllBytesFromPort()
{
    return current_port_.readAll();
}

///

BufferedPortFlowOperator::BufferedPortFlowOperator(QObject* parent):
    PortFlowOperator{parent},
    input_byte_buffer_{nullptr},
    output_byte_buffer_{nullptr}
{}

BufferedPortFlowOperator::BufferedPortFlowOperator(PortFlowSettings settings,
                                                   QSerialPortInfo port,
                                                   QObject* parent):
    PortFlowOperator{settings , port , parent}
{}

void BufferedPortFlowOperator::sendDataFromPortToBuffer()
{
    throwIf(input_byte_buffer_ == nullptr , "Byte buffer not initialized!");

    input_byte_buffer_->appendBytes(std::move(current_port_.readAll()));
    emit dataArrived();
}

void BufferedPortFlowOperator::sendDataFromBufferToPort()
{
    throwIf(output_byte_buffer_ == nullptr , "Byte buffer not initialized!");

    if(current_port_.isWritable())
        current_port_.write(output_byte_buffer_->getAllBytes());
}

void BufferedPortFlowOperator::setInputByteBuffer(ByteBuffer* byte_buffer)
{
    input_byte_buffer_ = byte_buffer;
    makeConnections();
}

void BufferedPortFlowOperator::setOutputByteBuffer(ByteBuffer* byte_buffer)
{
    output_byte_buffer_ = byte_buffer;
    makeConnections();
}

void BufferedPortFlowOperator::makeConnections()
{
    connect(&current_port_ , &QSerialPort::readyRead ,
            this , &BufferedPortFlowOperator::sendDataFromPortToBuffer );

    connect(output_byte_buffer_ , &port::ByteBuffer::bytesArrived ,
            this , &BufferedPortFlowOperator::sendDataFromBufferToPort );
}

}
