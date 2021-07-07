#include "../Converter.hpp"
#include <QMutexLocker>

PortScanner::PortScanner():
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

void DataHandler::appendReceivedBytes(QByteArray&& array)
{
    received_bytes_.append(std::move(array));
    emit bytesArrived(array.size());
}

QByteArray DataHandler::divideByteArray(size_t position)
{
    QMutexLocker lock{&data_mutex_};

    if(position + 1 > received_bytes_.size())
        throw std::logic_error{"requested count of bytes > received_bytes_.size()!\n"};

    QByteArray temp {std::move(received_bytes_.left(position))};
    received_bytes_ = std::move(received_bytes_.right(position));

    emit bytesExtracted(position + 1);
    return temp;
}

DataHandler::DataHandler(QObject *parent) :
    QObject{parent},
    received_bytes_{}
{}

QByteArray DataHandler::getAllReceivedBytes()
{
    return getReceivedBytes(received_bytes_.size());
}

QByteArray DataHandler::getReceivedBytes(size_t count)
{
    return divideByteArray(count - 1);
}

