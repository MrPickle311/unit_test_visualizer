#include "../PortSettings.hpp"

namespace backend
{

const QSerialPort::BaudRate& PortFlowSettings::baudRate() const
{
    return baud_rate_;
}

const QSerialPort::DataBits& PortFlowSettings::dataBits() const
{
    return data_bits_;
}

const QSerialPort::FlowControl& PortFlowSettings::flowControl() const
{
    return flow_control_;
}

const QSerialPort::Parity& PortFlowSettings::parity() const
{
    return parity_;
}

const QSerialPort::StopBits& PortFlowSettings::stopBits() const
{
    return stop_bits_;
}

void PortFlowSettings::setBaudRate(const QSerialPort::BaudRate& newBaud_rate)
{
    baud_rate_ = newBaud_rate;
}

void PortFlowSettings::setDataBits(const QSerialPort::DataBits& newData_bits)
{
    data_bits_ = newData_bits;
}

void PortFlowSettings::setFlowControl(const QSerialPort::FlowControl& newFlow_control)
{
    flow_control_ = newFlow_control;
}

void PortFlowSettings::setParity(const QSerialPort::Parity& newParity)
{
    parity_ = newParity;
}

void PortFlowSettings::setStopBits(const QSerialPort::StopBits& newStop_bits)
{
    stop_bits_ = newStop_bits;
}

PortFlowSettings::PortFlowSettings(const QSerialPort::BaudRate& baud_rate,
                                   const QSerialPort::DataBits& data_bits,
                                   const QSerialPort::FlowControl& flow_control,
                                   const QSerialPort::Parity& parity,
                                   const QSerialPort::StopBits& stop_bits) :
    baud_rate_(baud_rate),
    data_bits_(data_bits),
    flow_control_(flow_control),
    parity_(parity),
    stop_bits_(stop_bits)
{}

PortFlowSettings PortFlowSettings::cloneSettings() const
{
    return *this;
}

}

namespace global
{

template<>
void backend::StandardSettings::initValues()
{
    variables_[backend::StandardSetting::StandardSetting9600] =
    {QSerialPort::Baud9600 , QSerialPort::Data8 ,
     QSerialPort::NoFlowControl , QSerialPort::NoParity ,
     QSerialPort::OneStop};

    variables_[backend::StandardSetting::StandardSetting57600] =
    {QSerialPort::Baud57600 , QSerialPort::Data8 ,
     QSerialPort::NoFlowControl , QSerialPort::NoParity ,
     QSerialPort::OneStop};

    variables_[backend::StandardSetting::StandardSetting115200] =
    {QSerialPort::Baud115200 , QSerialPort::Data8 ,
     QSerialPort::NoFlowControl , QSerialPort::NoParity ,
     QSerialPort::OneStop};
}


}

