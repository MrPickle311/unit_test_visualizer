#include "../Converter.hpp"
#include <QMutexLocker>
#include <QDebug>

PortScanner::PortScanner(QObject* parent):
    QObject(parent),
    avalaible_ports_{QSerialPortInfo::availablePorts()}

{}

template<typename DataType>
QList<DataType> PortScanner::getSerialInfoStringList(SerialPortInfoMethod<DataType> method_to_call) const
{
    QList<DataType> data_list;

    for(auto&& avalaible_port : this->avalaible_ports_)
        data_list.push_back(method_to_call(avalaible_port));

    return data_list;
}

QSerialPortInfo PortScanner::getSelectedPort(uint port_nmbr) const
{
    if(port_nmbr >= static_cast<uint>(avalaible_ports_.size()))
        throw std::logic_error{"port_nmbr >= avalaible ports number!\n"};

    return avalaible_ports_[port_nmbr];
}

QList<int> PortScanner::getProductIndetifiers() const
{
    return getSerialInfoStringList<int>(&QSerialPortInfo::productIdentifier);
}

QList<QString> PortScanner::getPortNames() const
{
   return getSerialInfoStringList<QString>(&QSerialPortInfo::portName);
}

QList<QString> PortScanner::getPortDescriptions() const
{
    return getSerialInfoStringList<QString>(&QSerialPortInfo::description);
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

    if(count > received_bytes_.size())
        throw std::logic_error{"requested count of bytes > received_bytes_.size()!\n"};

    QByteArray temp {std::move(received_bytes_.left(count))};
    received_bytes_ = std::move(received_bytes_.right( received_bytes_.size() - count ));

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

StandardSettings::StandardSettings()
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

PortFlowSettings StandardSettings::getStandardSettings(StandardSetting setting) const
{
    return standard_settings_[setting];
}

//port operator

void PortOperator::setOpenMode(QIODevice::OpenMode open_mode)
{
    open_mode_ = open_mode;
}

PortOperator::PortOperator(QObject* parent):
    QObject{parent},
    current_port_{this},
    current_port_info_{},
    open_mode_{QSerialPort::NotOpen}
{}

void PortOperator::changePort(QSerialPortInfo port)
{
    current_port_info_ = port;
    current_port_.setPort(current_port_info_);
}

void PortOperator::changeSettings(PortFlowSettings settings)
{
    current_port_.setBaudRate(settings.baudRate());
    current_port_.setDataBits(settings.dataBits());
    current_port_.setParity(settings.parity());
    current_port_.setFlowControl(settings.flowControl());
    current_port_.setStopBits(settings.stopBits());
}

void PortOperator::closePort()
{
    if(current_port_.isOpen())
        current_port_.close();
    closeHook();
}

void PortOperator::openPort()
{
    if(current_port_.isOpen())
        throw std::logic_error{std::string{"This port(" } +
                               current_port_.portName().toStdString() +
                                       std::string{" ) is arleady open!"}};
    openHook();
    current_port_.open(this->open_mode_);
}

PortInputOperator::PortInputOperator(PortFlowSettings settings ,
                                     QSerialPortInfo  port     ,
                                     DataHandler*     data_handler ,
                                     QObject*         parent):
    PortOperator{parent},
    current_data_handler_{data_handler}
{
    changePort(port);
    changeSettings(settings);
    setOpenMode(QSerialPort::ReadOnly);

    connect(&current_port_ , &QSerialPort::readyRead ,
            this , &PortInputOperator::sendDataFromPortToHandler );
}

void PortInputOperator::setDataHandler(DataHandler* handler)
{
    current_data_handler_ = handler;
}

void PortInputOperator::sendDataFromPortToHandler()
{
    current_data_handler_->appendReceivedBytes(std::move(current_port_.readAll()));
}
