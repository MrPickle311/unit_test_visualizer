#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <functional>
#include <QSharedPointer>
#include <QMap>

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
    void                   rescan();
};

class DataHandler : public QObject
{
    Q_OBJECT;
private:
    QByteArray received_bytes_;
public:
    void       receiveBytes(QByteArray array);
    QByteArray getReceivedBytes();
};

class PortFlowSettings
{
    friend class PortOperator;
private:
    QSerialPort::BaudRate    baud_rate_;
    QSerialPort::DataBits    data_bits_;
    QSerialPort::FlowControl flow_control_;
    QSerialPort::Parity      parity_;
    QSerialPort::StopBits    stop_bits_;
public:
    PortFlowSettings(QSerialPort::BaudRate    baud_rate    ,
                     QSerialPort::DataBits    data_bits    ,
                     QSerialPort::FlowControl flow_control ,
                     QSerialPort::Parity      parity       ,
                     QSerialPort::StopBits    stop_bits  );
    PortFlowSettings cloneSettings() const;
};

enum class StandardSetting : size_t
{
    StandardSetting9600,
    StandardSetting115200
};

class StandardSettings
{
private:
    QMap<StandardSetting,PortFlowSettings> standard_settings_;
public:
    StandardSettings()
    {
        standard_settings_[StandardSetting::StandardSetting9600] =
        {QSerialPort::Baud9600 , QSerialPort::Data8 ,
         QSerialPort::NoFlowControl , QSerialPort::NoParity ,
         QSerialPort::OneStop};

        standard_settings_[StandardSetting::StandardSetting115200] =
        {QSerialPort::Baud115200 , QSerialPort::Data8 ,
         QSerialPort::NoFlowControl , QSerialPort::NoParity ,
         QSerialPort::OneStop};
    }

    PortFlowSettings getStandardSettings(StandardSetting setting) const;
};

class PortOperator : public QObject
{
    Q_OBJECT;
private:
    QSerialPort current_port_;
    QSharedPointer<const QSerialPortInfo> current_port_info_;//dependency
    QSharedPointer<DataHandler>           current_data_handler_;
    //reopen another port
public:
    PortOperator(const PortOperator& other);
    void openPort();
    void changePort(const QSerialPortInfo*);
    void setDataHandler(QSharedPointer<DataHandler> handler);
    void sendDataFromPortToHandler();
    void enableReceivingData();
    void disableReceivingData();
public slots:
    void applySettings(PortFlowSettings settings);
};

class PortOperatorCreator
{
private:
    QSerialPort::BaudRate baud_rate_;

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
