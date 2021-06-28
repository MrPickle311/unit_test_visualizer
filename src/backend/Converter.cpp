#include "../../include/backend/Converter.hpp"

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

const QSerialPortInfo* PortScanner::getSelectedPort(uint port_nmbr) const
{
    if(port_nmbr >= static_cast<uint>(avalaible_ports_.size()))
        throw std::logic_error{"port_nmbr >= avalaible ports number!\n"};

    QSharedDataPointer<const QSerialPortInfo> ptr {&avalaible_ports_.at(port_nmbr)};

    return &avalaible_ports_.at(port_nmbr);
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
