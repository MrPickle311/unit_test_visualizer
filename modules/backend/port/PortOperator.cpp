#include "../PortOperator.hpp"
#include <QDebug>

namespace port
{

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

void PortInputOperator::makeConnections()
{
    connect(&current_port_ , &QSerialPort::readyRead ,
            this , &PortInputOperator::sendDataFromPortToHandler );
}

PortInputOperator::PortInputOperator(QObject* parent):
    PortOperator{QSerialPort::ReadOnly , parent},
    current_data_handler_{nullptr}
{
    makeConnections();
}

PortInputOperator::PortInputOperator(PortFlowSettings settings ,
                                     QSerialPortInfo  port     ,
                                     DataHandler*     data_handler ,
                                     QObject*         parent):
    PortOperator{QSerialPort::ReadOnly , parent},
    current_data_handler_{data_handler}
{
    changePort(port);
    changeSettings(settings);
    makeConnections();
}

void PortInputOperator::setDataHandler(DataHandler* handler)
{
    current_data_handler_ = handler;
}

void PortInputOperator::sendDataFromPortToHandler()
{
    //TODO: logic error if handler_ == nullptr!!!
    current_data_handler_->appendReceivedBytes(std::move(current_port_.readAll()));
    emit dataArrived();
}


}
