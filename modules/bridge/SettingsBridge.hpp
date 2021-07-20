#pragma once

#include <QObject>
#include "../backend/PortOperator.hpp"
#include <QDebug>


class SettingsBridge:
        public QObject
{
    Q_OBJECT

    Q_PROPERTY(QSerialPort::Parity parity READ getParity WRITE setParity NOTIFY parityChanged);
    Q_PROPERTY(QSerialPort::StopBits stopBits READ getStopBits WRITE setStopBits NOTIFY stopBitsChanged);
    Q_PROPERTY(QSerialPort::DataBits dataBits READ getDataBits WRITE setDataBits NOTIFY dataBitsChanged);
    Q_PROPERTY(QSerialPort::BaudRate baudRate READ getBaudRate WRITE setBaudRate NOTIFY baudRateChanged);
    Q_PROPERTY(QStringList portNames READ getPortNames WRITE setPortNames NOTIFY portNamesChanged);

private:
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::DataBits dataBits;
    QSerialPort::BaudRate baudRate;
    QStringList portNames;
private:
    port::PortScanner scanner_;

public:
    explicit SettingsBridge(QObject *parent = nullptr);

    const QSerialPort::Parity& getParity() const;
    Q_INVOKABLE void setParity(const QSerialPort::Parity& newParity);

    const QSerialPort::StopBits& getStopBits() const;
    Q_INVOKABLE void setStopBits(const QSerialPort::StopBits& newStopBits);

    const QSerialPort::DataBits& getDataBits() const;
    Q_INVOKABLE void setDataBits(const QSerialPort::DataBits& newDataBits);

    const QSerialPort::BaudRate& getBaudRate() const;
    Q_INVOKABLE void setBaudRate(const QSerialPort::BaudRate& newBaudRate);

    Q_INVOKABLE void scanPorts();

    const QStringList& getPortNames() const;
    void setPortNames(const QStringList& newPortNames);

    Q_INVOKABLE void sendSettings(QString port_name);

signals:

    void parityChanged();
    void stopBitsChanged();
    void dataBitsChanged();
    void baudRateChanged();
    void portNamesChanged();

    void settingsApplied(QString port_name , port::PortFlowSettings settings);
};
