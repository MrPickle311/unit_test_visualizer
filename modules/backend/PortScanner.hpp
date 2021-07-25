#pragma once
#include <QStringList>
#include <QSerialPortInfo>
#include "../global/ProgramObject.hpp"
#include "Interfaces.hpp"

namespace backend
{

class PortScanner:
        public interface::PortScanner,
        public global::ProgramObject
{
private:
    QList<QSerialPortInfo> avalaible_ports_;
private:
    template<typename DataType>
    using SerialPortInfoMethod = std::function<DataType(const QSerialPortInfo& )>;

    template<typename DataType>
    QList<DataType> getSerialInfoList(SerialPortInfoMethod<DataType> method_to_call) const;
public:
    PortScanner();
    virtual ~PortScanner(){}
    virtual QSerialPortInfo getPortByNumber(uint port_nmbr)  const;
    virtual QSerialPortInfo getPortByName(QString port_name) const;
    virtual QList<int>      getProductIndetifiers()          const;
    virtual QStringList     getPortNames()                   const;
    virtual QStringList     getPortDescriptions()            const;
    virtual QStringList     getCompletePortData()            const;
    virtual void            rescan();
};


}
