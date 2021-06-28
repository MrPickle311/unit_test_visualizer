#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <functional>
#include <QSharedPointer>

class PortScanner : public QObject
{
    Q_OBJECT;
private:
    QList<QSerialPortInfo> avalaible_ports_;
private:
    template<typename DataType>
    using SerialPortInfoMethod = std::function<DataType(const QSerialPortInfo& )>;
    template<typename DataType>
    QList<DataType> getSerialInfoStringList(SerialPortInfoMethod<DataType> method_to_call) const;
public:
    PortScanner();
    const QSerialPortInfo* getSelectedPort(uint port_nmbr) const;
    QList<int>             getProductIndetifiers() const;
    QList<QString>         getPortNames() const;
    QList<QString>         getPortDescriptions() const;
};

class PortOperator : public QObject
{
    Q_OBJECT;
private:
    QSerialPort current_port_;
    QSharedPointer<const QSerialPortInfo> current_port_info_;//dependency
public:
};

class DataHandler : public QObject
{
    Q_OBJECT;
private:
    QByteArray received_bytes_;
public:
};

template<typename DataType>
struct DataPackage
{

};

class DataConverter : public QObject
{
    Q_OBJECT;
private:
    //create a something linke constructor which configures this object to certain data type
public:
   // static
};
