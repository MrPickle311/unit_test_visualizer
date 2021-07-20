#include "../PortSettings.hpp"
#include <QMutexLocker>
#include "../../global/GlobalFunctions.hpp"

namespace port
{

PortScanner::PortScanner(QObject* parent):
    QObject(parent),
    avalaible_ports_{QSerialPortInfo::availablePorts()}

{}

template<typename DataType>
QList<DataType> PortScanner::getSerialInfoList(SerialPortInfoMethod<DataType> method_to_call) const
{
    QList<DataType> data_list;

    for(auto&& avalaible_port : this->avalaible_ports_)
        data_list.push_back(method_to_call(avalaible_port));

    return data_list;
}

QSerialPortInfo PortScanner::getSelectedPort(uint port_nmbr) const
{
    throwIf(port_nmbr >= static_cast<uint>(avalaible_ports_.size()),
            "port_nmbr >= avalaible ports number!");

    return avalaible_ports_[port_nmbr];
}

QSerialPortInfo PortScanner::getPortByName(QString port_name) const
{
    for(auto&& port : avalaible_ports_)
        if(port.portName().contains(port_name))
            return port;
    return QSerialPortInfo{};
}

QList<int> PortScanner::getProductIndetifiers() const
{
    return getSerialInfoList<int>(&QSerialPortInfo::productIdentifier);
}

QStringList PortScanner::getPortNames() const
{
   return getSerialInfoList<QString>(&QSerialPortInfo::portName);
}

QStringList PortScanner::getPortDescriptions() const
{
    return getSerialInfoList<QString>(&QSerialPortInfo::description);
}

QStringList PortScanner::getCompletePortData() const
{
    return joinStringListElements( getPortNames() , getPortDescriptions() );
}

void PortScanner::rescan()
{
    avalaible_ports_.clear();
    avalaible_ports_ = QSerialPortInfo::availablePorts();
}



//port flow settings

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

void StandardSettings::initSettings()
{
    standard_settings_[StandardSetting::StandardSetting9600] =
    {QSerialPort::Baud9600 , QSerialPort::Data8 ,
     QSerialPort::NoFlowControl , QSerialPort::NoParity ,
     QSerialPort::OneStop};

    standard_settings_[StandardSetting::StandardSetting57600] =
    {QSerialPort::Baud57600 , QSerialPort::Data8 ,
     QSerialPort::NoFlowControl , QSerialPort::NoParity ,
     QSerialPort::OneStop};

    standard_settings_[StandardSetting::StandardSetting115200] =
    {QSerialPort::Baud115200 , QSerialPort::Data8 ,
     QSerialPort::NoFlowControl , QSerialPort::NoParity ,
     QSerialPort::OneStop};
}

StandardSettings::StandardSettings()
{}

QMutex StandardSettings::mutex_{};
QMap<StandardSetting,PortFlowSettings> StandardSettings::standard_settings_;
bool StandardSettings::settings_initialized_{false};


PortFlowSettings StandardSettings::getStandardSettings(StandardSetting setting)
{
    QMutexLocker lock{&mutex_};
    if(!settings_initialized_)
        initSettings();
    return standard_settings_[setting];
}

}

