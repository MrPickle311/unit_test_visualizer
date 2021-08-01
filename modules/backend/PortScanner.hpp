#pragma once
#include <QStringList>
#include <QSerialPortInfo>
#include "../global/ProgramObject.hpp"
#include "Interfaces.hpp"

namespace backend
{

///@brief Objects of this class are looking for avalaible RS232 ports.
///
/// This class contains list of avalaible RS232 ports. Scanning is performed
/// after constructor invocation or manually via rescan() method. Port  can
/// be accessed by its name or its position in the list. Object of his class
/// can provide string lists of :
/// - port name
/// - port description
/// - product identifier
/// - vendor  identifier
class PortScanner:
        public interface::PortScanner,
        public global::ProgramObject
{
private:
    QList<QSerialPortInfo> avalaible_ports_; // list of avalaible ports
private:
    template<typename DataType>
    using SerialPortInfoMethod = std::function<DataType(const QSerialPortInfo& )>;

    /// This is helper template method which applies method of QSerialPortInfo and
    /// applies it on each avalaible port. Results are aggregated in to list.
    /// @param method_to_call Address of QSerialPortInfo  method
    /// @return List of results of method_to_call applied on each port
    template<typename DataType>
    QList<DataType> getSerialInfoList(SerialPortInfoMethod<DataType> method_to_call) const;
public:
    ///@brief this default constructor perform scanning after be invoked
    PortScanner();
    virtual ~PortScanner(){}
    ///This method returns a port by passing its position on the list.
    /// @param port_nmbr Position port on the list
    /// @return QSerialPortInfo object with requested port.
    /// @attention port_nmbr must be less than portsCount() !
    /// @warning Positions on the list of the certain RS232 devices may differ on any PC!
    virtual QSerialPortInfo getPortByNumber(uint port_nmbr)  const;
    ///This method returns a port by passing its name
    /// @param port_name Name of an avalaible port
    /// @return QSerialPortInfo object with requested port.
    /// @warning Port names of the certain RS232 devices may differ on any PC!. For example
    /// same device may have COM6 name on one PC and COM4 on the other PC!
    virtual QSerialPortInfo getPortByName(QString port_name) const;
    /// @return This method returns list of product identifiers of avalaible ports.
    virtual QList<int>      getProductIndetifiers()          const;
    /// @return This method returns list of vendor identifiers of avalaible ports.
    virtual QList<int>      getVendorIndetifiers()           const;
    /// @return This method returns names of avalaible ports.
    virtual QStringList     getPortNames()                   const;
    /// @return This method returns descriptions of avalaible ports.
    virtual QStringList     getPortDescriptions()            const;
    /// @return This method returns all informations about of avalaible ports.
    virtual QStringList     getCompletePortData()            const;
    /// @return This method returns count of avalaible ports.
    virtual int             portsCount()                     const;
    ///This method allows us to scan system again and find new ports.
    virtual void            rescan();
};


}
