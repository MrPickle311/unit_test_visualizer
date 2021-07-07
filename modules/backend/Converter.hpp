#pragma once

#include <QSerialPort>
#include <QList>
#include <functional>
#include <QSharedPointer>
#include <QMap>
#include <QSerialPortInfo>
#include <QMutex>

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
    QSerialPortInfo  getSelectedPort(uint port_nmbr) const;
    QList<int>       getProductIndetifiers() const;
    QList<QString>   getPortNames() const;
    QList<QString>   getPortDescriptions() const;
    void             rescan();
};

class DataHandler : public QObject
{
    friend class PortOperator;
    Q_OBJECT;
private:
    QByteArray received_bytes_;
    QMutex     data_mutex_;
private:
    QByteArray divideByteArray(size_t position);
public:
    DataHandler(QObject *parent = nullptr);
    void       appendReceivedBytes(QByteArray&& array);
    QByteArray getAllReceivedBytes() noexcept;
    QByteArray getReceivedBytes(size_t count);
    bool       isEmpty() const;
    size_t     size()    const;
signals:
   void bytesArrived(size_t count);
   void bytesExtracted(size_t count);
};

//class PortFlowSettings
//{
//    friend class PortOperator;
//private:
//    QSerialPort::BaudRate    baud_rate_;
//    QSerialPort::DataBits    data_bits_;
//    QSerialPort::FlowControl flow_control_;
//    QSerialPort::Parity      parity_;
//    QSerialPort::StopBits    stop_bits_;
//public:
//    PortFlowSettings(QSerialPort::BaudRate    baud_rate    ,
//                     QSerialPort::DataBits    data_bits    ,
//                     QSerialPort::FlowControl flow_control ,
//                     QSerialPort::Parity      parity       ,
//                     QSerialPort::StopBits    stop_bits  );
//    PortFlowSettings(const PortFlowSettings& other);
//    PortFlowSettings cloneSettings() const;
//};
//
//enum class StandardSetting : size_t
//{
//    StandardSetting9600  ,
//    StandardSetting57600 ,
//    StandardSetting115200
//};
//
//class StandardSettings
//{
//private:
//    QMap<StandardSetting,PortFlowSettings> standard_settings_;
//public:
//    StandardSettings()
//    {
//        standard_settings_[StandardSetting::StandardSetting9600] =
//        {QSerialPort::Baud9600 , QSerialPort::Data8 ,
//         QSerialPort::NoFlowControl , QSerialPort::NoParity ,
//         QSerialPort::OneStop};
//
//        standard_settings_[StandardSetting::StandardSetting57600] =
//        {QSerialPort::Baud57600 , QSerialPort::Data8 ,
//         QSerialPort::NoFlowControl , QSerialPort::NoParity ,
//         QSerialPort::OneStop};
//
//        standard_settings_[StandardSetting::StandardSetting115200] =
//        {QSerialPort::Baud115200 , QSerialPort::Data8 ,
//         QSerialPort::NoFlowControl , QSerialPort::NoParity ,
//         QSerialPort::OneStop};
//    }
//
//    PortFlowSettings getStandardSettings(StandardSetting setting) const;
//};

//class PortOperator : public QObject
//{
//    Q_OBJECT;
//private:
//    QSerialPort current_port_;
//    QSharedPointer<const QSerialPortInfo> current_port_info_;//dependency
//
//    //reopen another port
//public:
//    PortOperator(const PortOperator& other);
//    void openPort();
//    void changePort(const QSerialPortInfo*);
//public slots:
//    void applySettings(PortFlowSettings settings);//only copy ,so nothing unexpected will happen
//};
//
//class PortOutputOperator : public PortOperator
//{
//    Q_OBJECT;
//    //i will implement it later
//};
//
//
//class PortInputOperator : public PortOperator
//{
//    Q_OBJECT;
//private:
//    QSharedPointer<DataHandler> current_data_handler_;
//public:
//    PortInputOperator(const PortInputOperator& other);
//    void enableReceivingData();
//    void disableReceivingData();
//    void setDataHandler(QSharedPointer<DataHandler> handler);
//public slots:
//    void sendDataFromPortToHandler(); //invoked automaticly
//};
//
////make error service as state machine
//
//class PortOperatorCreator
//{
//private:
//    PortFlowSettings flow_settings_;
//
//};
//
//////<-
//
//template<typename DataType>
//struct DataPackage
//{
//
//};
//
//class DataConverter : public QObject
//{
//    Q_OBJECT;
//private:
//    //create a something linke constructor which configures this object to certain data type
//public:
//   // static
//};
