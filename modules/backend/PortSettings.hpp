#pragma once

#include <QSerialPort>
#include <QList>
#include <functional>
#include <QSharedPointer>
#include <QMap>
#include <QSerialPortInfo>
#include <QMutex>
#include "../global/ProgramObject.hpp"
#include "../global/StaticGenerator.hpp"

namespace backend
{

class PortFlowSettings
{
private:
    QSerialPort::BaudRate    baud_rate_;
    QSerialPort::DataBits    data_bits_;
    QSerialPort::FlowControl flow_control_;
    QSerialPort::Parity      parity_;
    QSerialPort::StopBits    stop_bits_;
public:
    PortFlowSettings(const QSerialPort::BaudRate& baud_rate,
                     const QSerialPort::DataBits& data_bits,
                     const QSerialPort::FlowControl& flow_control,
                     const QSerialPort::Parity& parity,
                     const QSerialPort::StopBits& stop_bits);
    PortFlowSettings(const PortFlowSettings& other) = default;
    PortFlowSettings() = default;
public:
    PortFlowSettings cloneSettings() const;

    const QSerialPort::BaudRate& baudRate() const;
    const QSerialPort::DataBits& dataBits() const;
    const QSerialPort::FlowControl& flowControl() const;
    const QSerialPort::Parity& parity() const;
    const QSerialPort::StopBits& stopBits() const;
    void setBaudRate(const QSerialPort::BaudRate& newBaud_rate);
    void setDataBits(const QSerialPort::DataBits& newData_bits);
    void setFlowControl(const QSerialPort::FlowControl& newFlow_control);
    void setParity(const QSerialPort::Parity& newParity);
    void setStopBits(const QSerialPort::StopBits& newStop_bits);
};

enum class StandardSetting : size_t
{
    StandardSetting9600  ,
    StandardSetting57600 ,
    StandardSetting115200
};

using StandardSettings = global::StaticGenerator<StandardSetting , PortFlowSettings>;

}

namespace global
{

template<>
void backend::StandardSettings::initValues();

}
