#pragma once

#include <QObject>
#include <utility>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSharedPointer>
#include "../backend/PortSettings.hpp"
#include "../backend/Interfaces.hpp"
//#include <QDebug>

namespace bridge
{

/// @brief This is base class  for a terminal window direct binding
///
/// This abstract class provides also necessary signals to comunicate with UI
class TerminalBase
        : public QObject
{
    Q_OBJECT;
public slots:
    virtual void        applySettings(QSerialPortInfo port , backend::PortFlowSettings settings)      = 0;
    virtual void        openPort(QString port_name)                                                   = 0;
    virtual void        closeAllPorts()                                                               = 0;
    virtual QStringList restorePorts()                                                          const = 0;
    virtual void        sendData(QString port_name , QString data)                                    = 0;
signals:
    /// This signal is emitted after a port has been set
    /// @param port_name Name of port which has been set
    void newPortIsSet(QString port_name);
    ///  This signal is emitted after a data have been arrived from a port.
    /// @param port_name Name of port from which data arrives
    /// @param data Arriving data
    void dataArrived(QString port_name ,  QByteArray data);
    /// This signal is emitted after an error has occurred
    /// @param error Error object to send
    void errorOccurred(std::logic_error error);
};

/// @brief This class object is direct binding with terminal window in QML
///
/// It provides frontend <-> backend communication.
template<typename BufferType, typename PortOperatorType>
class Terminal:
        public TerminalBase
{
    using BufferPtr = QSharedPointer< BufferType >;
    using PortOperatorPtr = QSharedPointer<PortOperatorType>;
private:
    /// This helper function only changes settings of a existing port
    /// @param port_name Name of port to which a new setting should be applied
    /// @param settings New settings
    void changePortSettings(const QString& port_name , const backend::PortFlowSettings& settings)
    {
        if(set_ports_[port_name]->isOpen())
            set_ports_[port_name]->closePort();
        set_ports_[port_name]->changeSettings(settings);
    }
    /// This method is an engine to setting up a new ports. If a port is set up by the user
    /// and does not exists in the map then this method is invoked
    /// @param port Info about a new port to set up
    /// @param settings New settings
    void setupNewPort( const QSerialPortInfo& port , const backend::PortFlowSettings& settings)
    {
        QString port_name{port.portName()};

        //creating port operator
        set_ports_[port_name].reset(new PortOperatorType{settings , port , this});

        //creating buffers
        output_buffers_[port_name].reset(new BufferType{this});
        input_buffers_[port_name].reset(new BufferType{this});

        //binding buffers
        set_ports_[port_name]->setOutputByteBuffer(output_buffers_[port_name].get());
        set_ports_[port_name]->setInputByteBuffer(input_buffers_[port_name].get());

        //mapping signals , if signal port::ByteBuffer::bytesArrived(data) will arrive it is map
        //to dataArrived( port_name , data) and resend
        //port_name idientifies certain port and then QML knows where put data
        QObject::connect(input_buffers_[port_name].get() , &BufferType::bytesArrived ,
        [this , port_name]([[maybe_unused]] size_t count)
        {
            QByteArray bytes {input_buffers_[port_name]->getAllBytes()};
            emit dataArrived(port_name , bytes );
        });

        // error propagation from this port to outside
        QObject::connect(set_ports_[port_name].get() , &PortOperatorType::deviceErrorOccurred ,
        [this , port_name](QString what)
        {

            emit errorOccurred(std::logic_error{ (port_name + " " + what ).toStdString() });
        });

        emit newPortIsSet(port_name);
    }
private:
    QMap<QString , PortOperatorPtr > set_ports_; ///< ports which have been set up
    QMap<QString , BufferPtr>        input_buffers_;///< input buffers for each port operator
    QMap<QString , BufferPtr>        output_buffers_;///< output buffers for each port operator

public :
    /// This method is invoked from UI by user when he confirm port settings.
    /// This method can create a new set port or change settings for existing port
    /// @param port Info about a port to set up
    /// @param settings New settings
    virtual void applySettings(QSerialPortInfo port, backend::PortFlowSettings settings) override
    {
        QString port_name{port.portName()};

        if(set_ports_.contains(port_name))
            changePortSettings(port_name , settings);
        else setupNewPort(port, settings);
    }
    /// This method is invoked when user tries to open a set up port
    /// If port is already opened or any other error has happened then
    /// error is reported
    /// @param port_name Name of a port to open
    virtual void openPort(QString port_name) override
    {
        if(not set_ports_.contains(port_name))
            return;

        try
        {
            if(not set_ports_[port_name]->openPort())
                throw std::logic_error{" Error occured at opening port : " +
                                       set_ports_[port_name]->getError().toStdString() };
        }
        catch(const std::logic_error& error)
        {
            emit errorOccurred(error);
        }
    }
    /// This method is used to close all ports.
    /// It is invoked automatically when terminal windows is closing
    virtual void closeAllPorts() override
    {
        for(auto&& port : set_ports_)
            port->closePort();
    }
    /// This method is invoked when user reopens terminal window
    /// Method restores earlier set up ports to UI
    virtual QStringList restorePorts() const override
    {
        return set_ports_.keys();
    }
    /// This method is invoked when user tries to send data from terminal
    /// to port
    /// @param port_name Name of a port to which data should be send
    /// @param data Data to send
    virtual void sendData(QString port_name, QString data) override
    {
        output_buffers_[port_name]->appendBytes(data.toUtf8());//saviour
    }
};

}
