#include "../PortScanner.hpp"
#include "../../global/GlobalFunctions.hpp"

namespace backend
{

PortScanner::PortScanner():
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

QSerialPortInfo PortScanner::getPortByNumber(uint port_nmbr) const
{
    throwIf(port_nmbr >= static_cast<uint>(avalaible_ports_.size()),
            "port_nmbr >= avalaible ports number!");

    return avalaible_ports_[port_nmbr];
}

QSerialPortInfo PortScanner::getPortByName(QString port_name) const
{
    for(auto&& port : avalaible_ports_)
        if(port.portName().contains(port_name))
            return port;
    return QSerialPortInfo{};
}

QList<int> PortScanner::getProductIndetifiers() const
{
    return getSerialInfoList<int>(&QSerialPortInfo::productIdentifier);
}

QList<int> PortScanner::getVendorIndetifiers() const
{
    return getSerialInfoList<int>(&QSerialPortInfo::vendorIdentifier);
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
    return global::joinStringListElements( getPortNames() , getPortDescriptions() );
}

int PortScanner::portsCount() const
{
    return avalaible_ports_.size();
}

void PortScanner::rescan()
{
    avalaible_ports_.clear();
    avalaible_ports_ = QSerialPortInfo::availablePorts();
}


}
