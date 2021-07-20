#include "SettingsBridge.hpp"

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

const QSerialPort::DataBits& SettingsBridge::getDataBits() const
{
    return dataBits;
}

void SettingsBridge::setDataBits(const QSerialPort::DataBits& newDataBits)
{
    qDebug() << newDataBits;

    if (dataBits == newDataBits)
        return;
    dataBits = newDataBits;
    emit dataBitsChanged();
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

void SettingsBridge::scanPorts()
{
    scanner_.rescan();

    qDebug() << scanner_.getPortNames();

    setPortNames(scanner_.getPortNames());
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
    port::PortFlowSettings new_settings;

    new_settings.setBaudRate(baudRate);
    new_settings.setDataBits(dataBits);
    new_settings.setFlowControl(QSerialPort::NoFlowControl);
    new_settings.setParity(parity);
    new_settings.setStopBits(stopBits);

    qDebug() << "settingsApplied() " << scanner_.getPortByName(port_name).portName();

    emit settingsApplied(scanner_.getPortByName(port_name), new_settings);
}
