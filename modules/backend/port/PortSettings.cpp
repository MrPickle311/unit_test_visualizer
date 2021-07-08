#include "../PortSettings.hpp"
#include <QMutexLocker>

namespace port
{

PortScanner::PortScanner(QObject* parent):
    QObject(parent),
    avalaible_ports_{QSerialPortInfo::availablePorts()}

{}

template<typename DataType>
QList<DataType> PortScanner::getSerialInfoList(SerialPortInfoMethod<DataType> method_to_call) const
{
    QList<DataType> data_list;

    for(auto&& avalaible_port : this->avalaible_ports_)
        data_list.push_back(method_to_call(avalaible_port));

    return data_list;
}

template<typename... Args>
QStringList PortScanner::joinStringListElements(Args... args) const
{
    QStringList result;

    bool isBegin{true};

    auto appender = [&](QStringList list){
        for(int i{0} ; i < list.size() ; ++i)
        {
            if(isBegin)//its first invocation of appender(args) , allocate the memory for nodes
                result.push_back({});
            result[i] += " " + list[i];
        }

        isBegin = false;
    };

    (appender(args), ...);

    return result;
}


QSerialPortInfo PortScanner::getSelectedPort(uint port_nmbr) const
{
    if(port_nmbr >= static_cast<uint>(avalaible_ports_.size()))
        throw std::logic_error{"port_nmbr >= avalaible ports number!\n"};

    return avalaible_ports_[port_nmbr];
}

QList<int> PortScanner::getProductIndetifiers() const
{
    return getSerialInfoList<int>(&QSerialPortInfo::productIdentifier);
}

QStringList PortScanner::getPortNames() const
{
   return getSerialInfoList<QString>(&QSerialPortInfo::portName);
}

QStringList PortScanner::getPortDescriptions() const
{
    return getSerialInfoList<QString>(&QSerialPortInfo::description);
}

QStringList PortScanner::getCompletePortData() const
{
    return joinStringListElements(getPortNames() , getPortDescriptions() );
}

void PortScanner::rescan()
{
    avalaible_ports_.clear();
    avalaible_ports_ = QSerialPortInfo::availablePorts();
}

//data handler

void DataHandler::appendReceivedBytes(const QByteArray& array)
{
    QMutexLocker lock{&data_mutex_};
    received_bytes_.append(array);
    emit bytesArrived(array.size());
}

QByteArray DataHandler::divideByteArray(size_t count)
{
    QMutexLocker lock{&data_mutex_};

    if(static_cast<int>(count) > received_bytes_.size())
        throw std::logic_error{"requested count of bytes > received_bytes_.size()!\n"};

    QByteArray temp {received_bytes_.left(count)};
    received_bytes_ = received_bytes_.right( received_bytes_.size() - count );

    emit bytesExtracted(count );
    return temp;
}

DataHandler::DataHandler(QObject *parent) :
    QObject{parent},
    received_bytes_{}
{}

QByteArray DataHandler::getAllReceivedBytes() noexcept
{
    if(this->isEmpty())
        return QByteArray{};
    return getReceivedBytes(received_bytes_.size());
}

QByteArray DataHandler::getReceivedBytes(size_t count)
{
    return divideByteArray(count);
}

bool DataHandler::isEmpty() const
{
    return received_bytes_.isEmpty();
}

size_t DataHandler::size() const
{
    return received_bytes_.size();
}

//port flow settings

const QSerialPort::BaudRate& PortFlowSettings::baudRate() const
{
    return baud_rate_;
}

const QSerialPort::DataBits& PortFlowSettings::dataBits() const
{
    return data_bits_;
}

const QSerialPort::FlowControl& PortFlowSettings::flowControl() const
{
    return flow_control_;
}

const QSerialPort::Parity& PortFlowSettings::parity() const
{
    return parity_;
}

const QSerialPort::StopBits& PortFlowSettings::stopBits() const
{
    return stop_bits_;
}

PortFlowSettings::PortFlowSettings(const QSerialPort::BaudRate& baud_rate,
                                   const QSerialPort::DataBits& data_bits,
                                   const QSerialPort::FlowControl& flow_control,
                                   const QSerialPort::Parity& parity,
                                   const QSerialPort::StopBits& stop_bits) :
    baud_rate_(baud_rate),
    data_bits_(data_bits),
    flow_control_(flow_control),
    parity_(parity),
    stop_bits_(stop_bits)
{}

PortFlowSettings PortFlowSettings::cloneSettings() const
{
    return *this;
}

void StandardSettings::initSettings()
{
    standard_settings_[StandardSetting::StandardSetting9600] =
    {QSerialPort::Baud9600 , QSerialPort::Data8 ,
     QSerialPort::NoFlowControl , QSerialPort::NoParity ,
     QSerialPort::OneStop};

    standard_settings_[StandardSetting::StandardSetting57600] =
    {QSerialPort::Baud57600 , QSerialPort::Data8 ,
     QSerialPort::NoFlowControl , QSerialPort::NoParity ,
     QSerialPort::OneStop};

    standard_settings_[StandardSetting::StandardSetting115200] =
    {QSerialPort::Baud115200 , QSerialPort::Data8 ,
     QSerialPort::NoFlowControl , QSerialPort::NoParity ,
     QSerialPort::OneStop};
}

StandardSettings::StandardSettings()
{}

QMutex StandardSettings::mutex_{};
QMap<StandardSetting,PortFlowSettings> StandardSettings::standard_settings_;
bool StandardSettings::settings_initialized_{false};


PortFlowSettings StandardSettings::getStandardSettings(StandardSetting setting)
{
    QMutexLocker lock{&mutex_};
    if(!settings_initialized_)
        initSettings();
    return standard_settings_[setting];
}

}
