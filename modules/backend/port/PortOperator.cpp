#include "../PortOperator.hpp"
#include <QDebug>

namespace port
{

//data handler

void ByteBuffer::appendBytes(const QByteArray& array)
{
    QMutexLocker lock{&data_mutex_};
    received_bytes_.append(array);
    lock.unlock();

    emit bytesArrived(array.size());
}

void ByteBuffer::appendByte(char byte)
{
    received_bytes_.append(byte);

    emit bytesArrived(1);//one byte
}

QByteArray ByteBuffer::splitByteArray(size_t count)
{
    QMutexLocker lock{&data_mutex_};

    throwIf(static_cast<int>(count) > received_bytes_.size(),
            "requested count of bytes > received_bytes_.size()!\n");

    QByteArray temp {received_bytes_.left(count)};
    received_bytes_ = received_bytes_.right( received_bytes_.size() - count );

    lock.unlock();

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

char ByteBuffer::getByte()
{
    return getBytes(1).front();
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
    current_port_.setPort(port);
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

bool PortStateOperator::isOpen() const
{
    return current_port_.isOpen();
}

QSerialPort::SerialPortError PortStateOperator::getError() const
{
    return current_port_.error();
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
    checkBuffer(input_byte_buffer_);

    input_byte_buffer_->appendBytes(std::move(current_port_.readAll()));
    emit dataArrived();
}

void BufferedPortFlowOperator::sendDataFromBufferToPort()
{
    checkBuffer(output_byte_buffer_);

    if(current_port_.isWritable())
        current_port_.write(output_byte_buffer_->getAllBytes());

    emit dataSent();
}

void BufferedPortFlowOperator::setInputByteBuffer(ByteBuffer* byte_buffer)
{
    checkBuffer(byte_buffer);

    input_byte_buffer_ = byte_buffer;
    makeInputBufferConnections();
}

void BufferedPortFlowOperator::setOutputByteBuffer(ByteBuffer* byte_buffer)
{
    checkBuffer(byte_buffer);

    output_byte_buffer_ = byte_buffer;
    makeOutputBufferConnections();
}

void BufferedPortFlowOperator::makeInputBufferConnections()
{
    connect(&current_port_ , &QSerialPort::readyRead ,
            this , &BufferedPortFlowOperator::sendDataFromPortToBuffer );
}

void BufferedPortFlowOperator::makeOutputBufferConnections()
{
    connect(output_byte_buffer_ , &port::ByteBuffer::bytesArrived ,
            this , &BufferedPortFlowOperator::sendDataFromBufferToPort );
}

void BufferedPortFlowOperator::checkBuffer(ByteBuffer* buffer) noexcept(false)
{
    throwIf(buffer == nullptr , "Byte buffer is nullptr!");
}

}
