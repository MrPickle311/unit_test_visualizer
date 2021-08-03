#pragma once

#include <QObject>
#include <QDebug>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSharedPointer>
#include "../backend/PortSettings.hpp"
#include "../backend/Interfaces.hpp"

namespace bridge
{

/// @brief This class is proxy for the types compatible with interface::PortScanner
/// interface.
///
/// This proxy provides signals and slots feature for the wrapped object, so it allows
/// to communication with other objects without dependencies
class Scanner:
        public QObject
{
    Q_OBJECT;
private:
    QSharedPointer<interface::PortScanner> scanner_impl_;

public:
    /// Default constructor
    /// @param scanner_impl Core port scanner implementation
    /// @param parent QObject parent pointer
    Scanner(QSharedPointer<interface::PortScanner> scanner_impl , QObject* parent = nullptr);

public slots:
    /// This method scans for avalaible ports.
    void            scanPorts();//invoked from QML
    ///This method returns a port by passing its name
    /// @param port_name Name of an avalaible port
    /// @return QSerialPortInfo object with requested port.
    QSerialPortInfo getPortByName(QString port_name);

signals:
    /// This signal is emiited after port scanning
    /// @param newPortNames Port names to send
    void portsScanned(const QStringList& newPortNames);

};

/// @brief This class is common base for other settings classes
///
/// This class is base for direct bindings with QML interface.
/// When user manipulate the interface then implicity manipulate
/// object of this class by changing its properties such as stopBits.
/// It has possibility to sending collected settings information
/// to other objects via signal/slot system
class Settings:
        public QObject
{
    Q_OBJECT

    Q_PROPERTY(QSerialPort::Parity      parity      READ getParity       WRITE setParity      NOTIFY parityChanged);
    Q_PROPERTY(QSerialPort::StopBits    stopBits    READ getStopBits     WRITE setStopBits    NOTIFY stopBitsChanged);
    Q_PROPERTY(QSerialPort::BaudRate    baudRate    READ getBaudRate     WRITE setBaudRate    NOTIFY baudRateChanged);
    Q_PROPERTY(QSerialPort::FlowControl flowControl READ getFlowControl  WRITE setFlowControl NOTIFY flowControlChanged);
    Q_PROPERTY(QStringList              portNames   READ getPortNames    WRITE setPortNames   NOTIFY portNamesChanged);

protected:
    QSerialPort::Parity       parity;
    QSerialPort::StopBits     stopBits;
    QSerialPort::BaudRate     baudRate;
    QSerialPort::FlowControl  flowControl;
    QStringList               portNames;
protected:
    virtual backend::PortFlowSettings prepareSettings() const = 0;

public :
    /// Default constructor
    /// @param parent QObject parent pointer
    explicit Settings(QObject *parent = nullptr);

public slots:
    /// @return Returns currently set parity
    const QSerialPort::Parity&   getParity()    const;
    /// @return Returns currently set stop bits
    const QSerialPort::StopBits& getStopBits()  const;
    /// @return Returns currently set baud rate
    const QSerialPort::BaudRate& getBaudRate()  const;
    /// @return Returns currently set flow control
    QSerialPort::FlowControl     getFlowControl() const;
    /// @return Returns list of names of found RS232 ports
    const QStringList&           getPortNames() const;

    /// @param newParity Parity to set
    void setParity(const QSerialPort::Parity& newParity);
    /// @param newStopBits Stop bits to set
    void setStopBits(const QSerialPort::StopBits& newStopBits);
    /// @param newBaudRate Baud rate to set
    void setBaudRate(const QSerialPort::BaudRate& newBaudRate);
    /// @param newPortNames Port names to set
    void setPortNames(const QStringList& newPortNames);
    /// @param flowControl Flow control to set
    void setFlowControl(QSerialPort::FlowControl flowControl);

    /// This method sends settings to a port with specified name.
    /// It uses settingsApplied() signal to send the settings
    /// @param port_name Name of port to which send settings
    void sendSettings(QString port_name);



signals:

    /// This signal is emitted after parity setting has been changed
    void parityChanged();
    /// This signal is emitted after stop bits setting have been changed
    void stopBitsChanged();
    /// This signal is emitted after baud rate setting has been changed
    void baudRateChanged();
    /// This signal is emitted after port names list has been changed
    void portNamesChanged();
    /// This signal is emitted after flow control setting has been changed
    void flowControlChanged(QSerialPort::FlowControl flowControl);

    /// This signal is emitted after the settings has been prepared to send
    /// by prepareSettings() method.
    /// @param port Port to which apply settings
    /// @param settings Settings to send
    void settingsApplied(QSerialPortInfo port , backend::PortFlowSettings settings);

    /// This signals is used to obtain port info object from other external
    /// object by its name
    /// @param port_name Name of requested port info
    /// @return Returns a port information object
    QSerialPortInfo portRequest(QString port_name);

};

/// @brief This class object is direct binding with terminal settings window in QML
///
/// It grabs all informtations from QML interface put to object of this type.
class TerminalSettings:
        public Settings
{
    Q_OBJECT;

    Q_PROPERTY(QSerialPort::DataBits dataBits READ getDataBits WRITE setDataBits NOTIFY dataBitsChanged);

private:
    QSerialPort::DataBits dataBits;

public slots:
    /// @return Returns currently set data bits
    const QSerialPort::DataBits& getDataBits() const;
    /// @param newDataBits Data bits to set
    void setDataBits(const QSerialPort::DataBits& newDataBits);

signals :
    /// This signal is emitted after data bits have been changed
    void dataBitsChanged();

protected:

    /// This method packs settings from properties into PortFlowSettings object
    /// @return Returns port settings object
    virtual backend::PortFlowSettings prepareSettings() const override;
};


/// @brief This class object is direct binding with tests settings window in QML
///
/// It grabs all informtations from QML interface put to object of this type.
class TestsSettings:
        public Settings
{
    Q_OBJECT;
protected:
    /// This method packs settings from properties into PortFlowSettings object
    /// @return Returns port settings object
    virtual backend::PortFlowSettings prepareSettings() const override;
};

}
