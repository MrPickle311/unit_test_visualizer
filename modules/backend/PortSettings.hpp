#pragma once

#include <QSerialPort>
#include <QList>
#include <functional>
#include <QSharedPointer>
#include <QMap>
#include <QSerialPortInfo>
#include <QMutex>
#include "../global/ProgramObject.hpp"

namespace port
{

class PortScanner :
        public QObject,
        public ProgramObject
{
    Q_OBJECT;
private:
    QList<QSerialPortInfo> avalaible_ports_;
private:
    template<typename DataType>
    using SerialPortInfoMethod = std::function<DataType(const QSerialPortInfo& )>;
    template<typename DataType>
    QList<DataType> getSerialInfoList(SerialPortInfoMethod<DataType> method_to_call) const;
public:
    PortScanner(QObject* parent = nullptr);
    virtual ~PortScanner(){}
    QSerialPortInfo getSelectedPort(uint port_nmbr) const;
    QSerialPortInfo getPortByName(QString port_name) const;
    QList<int>      getProductIndetifiers() const;
    QStringList     getPortNames() const;
    QStringList     getPortDescriptions() const;
    QStringList     getCompletePortData() const;
    void            rescan();
};

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

class StandardSettings
{
private:
    static QMutex mutex_;
    static QMap<StandardSetting,PortFlowSettings> standard_settings_;
    static bool settings_initialized_;
private:
    static void initSettings();
protected:
    StandardSettings();
public:
    static PortFlowSettings getStandardSettings(StandardSetting setting);
};

}
