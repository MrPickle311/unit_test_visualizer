#pragma once

#include <QObject>
#include <QDebug>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSharedPointer>
#include "../backend/PortSettings.hpp"
#include "../backend/Interfaces.hpp"

namespace bridge
{

class Scanner:
        public QObject
{
    Q_OBJECT;
private:
    QSharedPointer<interface::PortScanner> scanner_impl_;

public:
    Scanner(QSharedPointer<interface::PortScanner> scanner_impl , QObject* parent = nullptr);

public slots:
    void scanPorts();//invoked from QML
    QSerialPortInfo getPortByName(QString port_name);

signals:
    void portsScanned(const QStringList& newPortNames);

};

//common base for other settings
class Settings:
        public QObject
{
    Q_OBJECT

    Q_PROPERTY(QSerialPort::Parity parity READ getParity WRITE setParity NOTIFY parityChanged);
    Q_PROPERTY(QSerialPort::StopBits stopBits READ getStopBits WRITE setStopBits NOTIFY stopBitsChanged);
    Q_PROPERTY(QSerialPort::BaudRate baudRate READ getBaudRate WRITE setBaudRate NOTIFY baudRateChanged);
    Q_PROPERTY(QStringList portNames READ getPortNames WRITE setPortNames NOTIFY portNamesChanged);

protected:
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::BaudRate baudRate;

    QStringList portNames;
protected:
    virtual backend::PortFlowSettings prepareSettings() const = 0;

public :
    explicit Settings(QObject *parent = nullptr);

public slots:
    const QSerialPort::Parity& getParity() const;
    void setParity(const QSerialPort::Parity& newParity);

    const QSerialPort::StopBits& getStopBits() const;
    void setStopBits(const QSerialPort::StopBits& newStopBits);

    const QSerialPort::BaudRate& getBaudRate() const;
    void setBaudRate(const QSerialPort::BaudRate& newBaudRate);

    const QStringList& getPortNames() const;
    void setPortNames(const QStringList& newPortNames);

    void sendSettings(QString port_name);

signals:

    void parityChanged();
    void stopBitsChanged();
    void baudRateChanged();
    void portNamesChanged();

    void settingsApplied(QSerialPortInfo port , backend::PortFlowSettings settings);

    QSerialPortInfo portRequest(QString port_name);
};



class TerminalSettings:
        public Settings
{
    Q_OBJECT;

    Q_PROPERTY(QSerialPort::DataBits dataBits READ getDataBits WRITE setDataBits NOTIFY dataBitsChanged);

private:
    QSerialPort::DataBits dataBits;

public slots:
    const QSerialPort::DataBits& getDataBits() const;
    void setDataBits(const QSerialPort::DataBits& newDataBits);

signals :
    void dataBitsChanged();

protected:
    virtual backend::PortFlowSettings prepareSettings() const override;
};

class TestsSettings:
        public Settings
{
    Q_OBJECT;
protected:
    virtual backend::PortFlowSettings prepareSettings() const override;
};

}
