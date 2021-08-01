#pragma once

#include <QSerialPort>
#include <QList>
#include <functional>
#include <QSharedPointer>
#include <QMap>
#include <QSerialPortInfo>
#include <QMutex>
#include "../global/ProgramObject.hpp"
#include "../global/StaticGenerator.hpp"

namespace backend
{

/// @brief This class contains settings for a port.
///
/// This is package which can be
/// passed to
/// backend::PortStateOperator::changeSettings()
/// to change current port settings.
/// This class provides getters and setters for each setting.
/// This class allows to set the following options:
/// - baud rate
/// - data bits
/// - flow control
/// - parity bits
/// - stop bits
///
/// @see StandardSettings StandardSetting
class PortFlowSettings
{
private:
    QSerialPort::BaudRate    baud_rate_;
    QSerialPort::DataBits    data_bits_;
    QSerialPort::FlowControl flow_control_;
    QSerialPort::Parity      parity_;
    QSerialPort::StopBits    stop_bits_;
public:
    /// @brief Constructor which explicitly accepts all port options
    PortFlowSettings(const QSerialPort::BaudRate& baud_rate,
                     const QSerialPort::DataBits& data_bits,
                     const QSerialPort::FlowControl& flow_control,
                     const QSerialPort::Parity& parity,
                     const QSerialPort::StopBits& stop_bits);

    PortFlowSettings(const PortFlowSettings& other) = default;
    /// @brief Default constructor
    PortFlowSettings()                              = default;
public:
    /// @brief This function returns a copy of the current PortFlowSettings instance.
    PortFlowSettings                cloneSettings()                                                 const;
    /// @return Returns the currently set baud rate
    const QSerialPort::BaudRate&    baudRate()                                                      const;
    /// @return Returns the currently set data bits
    const QSerialPort::DataBits&    dataBits()                                                      const;
    /// @return Returns the currently set flow control
    const QSerialPort::FlowControl& flowControl()                                                   const;
    /// @return Returns the currently set parity
    const QSerialPort::Parity&      parity()                                                        const;
    /// @return Returns the currently set stop bits
    const QSerialPort::StopBits&    stopBits()                                                      const;
    /// @param new_baud_rate baud rate to set
    void                            setBaudRate(const QSerialPort::BaudRate& new_baud_rate);
    /// @param new_data_bits data bits to set
    void                            setDataBits(const QSerialPort::DataBits& new_data_bits);
    /// @param new_flow_control flow control to set
    void                            setFlowControl(const QSerialPort::FlowControl& new_flow_control);
    /// @param new_parity parity to set
    void                            setParity(const QSerialPort::Parity& new_parity);
    /// @param new_stop_bits stop bits to set
    void                            setStopBits(const QSerialPort::StopBits& new_stop_bits);
};

///This enum holds standard settings keys . They can be used to get access
///to a PortFlowSettings object.
///
/// @see StandardSettings PortFlowSettings

enum class StandardSetting : size_t
{
    StandardSetting9600  ,///< 9600 baud rate
    StandardSetting57600 ,///< 57600 baud rate
    StandardSetting115200 ///< 115200 baud rate
};

/// This is generator which holds most frauqently used port settings.
/// It stores these settings in PortFlowSettings objects.
/// Each stored setting can be accesed by a key of StandardSetting enum type.
/// Every standard settings have common options:
/// - 8 data bits
/// - no flow control
/// - 1 stop bit
/// - no parity
///
/// They differ only in baud rate
/// @see StandardSettings PortFlowSettings
using StandardSettings = global::StaticGenerator<StandardSetting , PortFlowSettings>;

}

namespace global
{

template<>
void backend::StandardSettings::initValues();

}
