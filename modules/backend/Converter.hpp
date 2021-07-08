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
    PortScanner(QObject* parent = nullptr);
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
    void       appendReceivedBytes(const QByteArray& array);
    QByteArray getAllReceivedBytes() noexcept;
    QByteArray getReceivedBytes(size_t count);
    bool       isEmpty() const;
    size_t     size()    const;
signals:
   void bytesArrived(size_t count);
   void bytesExtracted(size_t count);
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
    PortFlowSettings cloneSettings() const;
    //getters
    const QSerialPort::BaudRate& baudRate() const;
    const QSerialPort::DataBits& dataBits() const;
    const QSerialPort::FlowControl& flowControl() const;
    const QSerialPort::Parity& parity() const;
    const QSerialPort::StopBits& stopBits() const;
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

QMutex StandardSettings::mutex_{};
QMap<StandardSetting,PortFlowSettings> StandardSettings::standard_settings_;
bool StandardSettings::settings_initialized_{false};

//one operator per one port
class PortOperator : public QObject//it only opens a port , nothing else
{
    Q_OBJECT;
protected:
    QSerialPort           current_port_;
    QSerialPortInfo       current_port_info_;
    QSerialPort::OpenMode open_mode_;
protected:
    virtual void openHook(){};
    virtual void closeHook(){};
    void         setOpenMode(QSerialPort::OpenMode open_mode);
public:
    PortOperator(QSerialPort::OpenMode open_mode, QObject* parent);
public slots:
    void changePort(QSerialPortInfo port);
    void changeSettings(PortFlowSettings settings);//only copy ,so nothing unexpected will happen
    void closePort();
    void openPort();
};

//class PortOutputOperator : public PortOperator
//{
//    Q_OBJECT;
//    //i will implement it later
//};

class PortInputOperator : public PortOperator
{
    Q_OBJECT;
private:
    DataHandler* current_data_handler_;
private:
    void makeConnections();
public:
    PortInputOperator(QObject* parent = nullptr);
    PortInputOperator(PortFlowSettings settings ,
                      QSerialPortInfo  port     ,
                      DataHandler* data_handler ,
                      QObject* parent = nullptr);
public:
    void setDataHandler(DataHandler* handler);
public slots:
    void sendDataFromPortToHandler(); //invoked automatically
};



/// BELOW CODE IS GOING TO ANOTHER FILE
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
