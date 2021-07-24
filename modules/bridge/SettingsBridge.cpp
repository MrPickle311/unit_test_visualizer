#include "SettingsBridge.hpp"

namespace bridge
{

Settings::Settings(QObject *parent) : QObject(parent)
{

}

const QSerialPort::Parity& Settings::getParity() const
{
    return parity;
}

void Settings::setParity(const QSerialPort::Parity& newParity)
{
    if (parity == newParity)
        return;
    parity = newParity;
    emit parityChanged();
}

const QSerialPort::StopBits& Settings::getStopBits() const
{
    return stopBits;
}

void Settings::setStopBits(const QSerialPort::StopBits& newStopBits)
{
    if (stopBits == newStopBits)
        return;
    stopBits = newStopBits;
    emit stopBitsChanged();
}

const QSerialPort::BaudRate& Settings::getBaudRate() const
{
    return baudRate;
}

void Settings::setBaudRate(const QSerialPort::BaudRate& newBaudRate)
{
    if (baudRate == newBaudRate)
        return;
    baudRate = newBaudRate;
    emit baudRateChanged();
}

Scanner::Scanner(QSharedPointer<backend::PortScannerInterface> scanner_impl, QObject* parent):
    QObject{parent},
    scanner_impl_{scanner_impl}
{
    //THROW IF scanner_impl == NULL
}

void Scanner::scanPorts()
{
    scanner_impl_->rescan();

    emit  portsScanned(scanner_impl_->getPortNames());
}

QSerialPortInfo Scanner::getPortByName(QString port_name)
{
    return scanner_impl_->getPortByName(port_name);
}

const QStringList& Settings::getPortNames() const
{
    return portNames;
}

void Settings::setPortNames(const QStringList& newPortNames)
{
    if (portNames == newPortNames)
        return;
    portNames = newPortNames;
    emit portNamesChanged();
}

void Settings::sendSettings(QString port_name)
{
    backend::PortFlowSettings new_settings{prepareSettings()};

    emit settingsApplied( emit portRequest(port_name), new_settings);
}

///terminal settings

const QSerialPort::DataBits& TerminalSettings::getDataBits() const
{
    return dataBits;
}

void TerminalSettings::setDataBits(const QSerialPort::DataBits& newDataBits)
{
    if (dataBits == newDataBits)
        return;
    dataBits = newDataBits;
    emit dataBitsChanged();
}


backend::PortFlowSettings TerminalSettings::prepareSettings() const
{
    backend::PortFlowSettings new_settings;

    new_settings.setBaudRate(baudRate);
    new_settings.setDataBits(dataBits);
    new_settings.setFlowControl(QSerialPort::NoFlowControl);
    new_settings.setParity(parity);
    new_settings.setStopBits(stopBits);

    return new_settings;
}

backend::PortFlowSettings TestsSettings::prepareSettings() const
{
    backend::PortFlowSettings new_settings;

    new_settings.setBaudRate(baudRate);
    new_settings.setDataBits(QSerialPort::Data8);
    new_settings.setFlowControl(QSerialPort::NoFlowControl);
    new_settings.setParity(QSerialPort::NoParity);
    new_settings.setStopBits(QSerialPort::OneStop);

    return new_settings;
}

}
