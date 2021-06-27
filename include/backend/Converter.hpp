#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <functional>

class PortScanner : public QObject
{
    Q_OBJECT;
private:
    QList<QSerialPortInfo> avalaible_ports_;
private:
    template<typename DataType>
    QList<DataType> getSerialInfoStringList(std::function<DataType(const QSerialPortInfo& )> method_to_call) const;
public:
    PortScanner();
    QSerialPortInfo getSelectedPort(uint port_nmbr) const;
    QList<int>  getProductIndetifiers() const;
    QList<QString>  getPortNames() const;
    QList<QString>  getPortDescriptions() const;
};

class DataHandler : public QObject
{
    Q_OBJECT;
private:

public:
};

class DataConverter : public QObject
{
    Q_OBJECT;
private:

public:
};
