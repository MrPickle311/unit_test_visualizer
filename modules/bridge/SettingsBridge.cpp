#include "SettingsBridge.hpp"

namespace bridge
{

SettingsBridge::SettingsBridge(QObject *parent) : QObject(parent)
{

}

const QSerialPort::Parity& SettingsBridge::getParity() const
{
    return parity;
}

void SettingsBridge::setParity(const QSerialPort::Parity& newParity)
{
    qDebug() << newParity;

    if (parity == newParity)
        return;
    parity = newParity;
    emit parityChanged();
}

const QSerialPort::StopBits& SettingsBridge::getStopBits() const
{
    return stopBits;
}

void SettingsBridge::setStopBits(const QSerialPort::StopBits& newStopBits)
{
    qDebug() << newStopBits;

    if (stopBits == newStopBits)
        return;
    stopBits = newStopBits;
    emit stopBitsChanged();
}

const QSerialPort::BaudRate& SettingsBridge::getBaudRate() const
{
    return baudRate;
}

void SettingsBridge::setBaudRate(const QSerialPort::BaudRate& newBaudRate)
{
    qDebug() << newBaudRate;

    if (baudRate == newBaudRate)
        return;
    baudRate = newBaudRate;
    emit baudRateChanged();
}

Scanner::Scanner(QSharedPointer<port::PortScannerInterface> scanner_impl, QObject* parent):
    QObject{parent},
    scanner_impl_{scanner_impl}
{
    //THROW IF scanner_impl == NULL
}

void Scanner::scanPorts()
{
    scanner_impl_->rescan();

    qDebug() << scanner_impl_->getPortNames();

    emit  portsScanned(scanner_impl_->getPortNames());
}

QSerialPortInfo Scanner::getPortByName(QString port_name)
{
    return scanner_impl_->getPortByName(port_name);
}

const QStringList& SettingsBridge::getPortNames() const
{
    return portNames;
}

void SettingsBridge::setPortNames(const QStringList& newPortNames)
{
    if (portNames == newPortNames)
        return;
    portNames = newPortNames;
    emit portNamesChanged();
}

void SettingsBridge::sendSettings(QString port_name)
{
    port::PortFlowSettings new_settings{prepareSettings()};

    qDebug() << "settingsApplied() " <<  ( emit portRequest(port_name) ).portName() ;

    emit settingsApplied( emit portRequest(port_name), new_settings);
}

///terminal settings

const QSerialPort::DataBits& TerminalSettingsBridge::getDataBits() const
{
    return dataBits;
}

void TerminalSettingsBridge::setDataBits(const QSerialPort::DataBits& newDataBits)
{
    qDebug() << newDataBits;

    if (dataBits == newDataBits)
        return;
    dataBits = newDataBits;
    emit dataBitsChanged();
}


port::PortFlowSettings TerminalSettingsBridge::prepareSettings() const
{
    port::PortFlowSettings new_settings;

    new_settings.setBaudRate(baudRate);
    new_settings.setDataBits(dataBits);
    new_settings.setFlowControl(QSerialPort::NoFlowControl);
    new_settings.setParity(parity);
    new_settings.setStopBits(stopBits);

    return new_settings;
}

port::PortFlowSettings TestsSettingsBridge::prepareSettings() const
{
    port::PortFlowSettings new_settings;

    new_settings.setBaudRate(baudRate);
    new_settings.setDataBits(QSerialPort::Data8);
    new_settings.setFlowControl(QSerialPort::NoFlowControl);
    new_settings.setParity(QSerialPort::NoParity);
    new_settings.setStopBits(QSerialPort::OneStop);

    return new_settings;
}

}
