#pragma once

#include <QObject>
#include <utility>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSharedPointer>
#include "../backend/PortSettings.hpp"
#include "../backend/Interfaces.hpp"

namespace bridge
{

class TerminalBase
        : public QObject
{
    Q_OBJECT;
public slots:
    virtual void        applySettings(QSerialPortInfo port , backend::PortFlowSettings settings)      = 0;
    virtual void        openPort(QString port_name)                                                   = 0;
    virtual void        closeAllPorts()                                                               = 0;
    virtual QStringList restorePorts()                                                          const = 0;
    virtual void        sendData(QString port_name , QByteArray data)                                 = 0;
signals:
    void newPortIsSet(QString port_name);
    void dataArrived(QString port_name ,  QByteArray data);
    void errorOccurred(std::logic_error error);
};

//this template accepts implementations
template<typename BufferType, typename PortOperatorType>
class Terminal:
        public TerminalBase
{
    using BufferPtr = QSharedPointer< BufferType >;
    using PortOperatorPtr = QSharedPointer<PortOperatorType>;
private:
    void changePortSettings(const QString& port_name , const backend::PortFlowSettings& settings)
    {
        if(set_ports_[port_name]->isOpen())
            set_ports_[port_name]->closePort();
        set_ports_[port_name]->changeSettings(settings);
    }
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
        QObject::connect(input_buffers_[port_name].get() , &interface::ByteBuffer::bytesArrived ,
        [this , port_name]([[maybe_unused]] size_t count)
        {
            emit dataArrived(port_name , input_buffers_[port_name]->getAllBytes() );
        });

        QObject::connect(set_ports_[port_name].get() , &PortOperatorType::deviceErrorOccurred ,
        [this , port_name](QString what)
        {
            emit errorOccurred(std::logic_error{ (port_name + " " + what ).toStdString() });
        });

        emit newPortIsSet(port_name);
    }
private:
    QMap<QString , PortOperatorPtr > set_ports_;
    QMap<QString , BufferPtr>        input_buffers_;
    QMap<QString , BufferPtr>        output_buffers_;

public :
    virtual void applySettings(QSerialPortInfo port, backend::PortFlowSettings settings) override
    {
        QString port_name{port.portName()};

        if(set_ports_.contains(port_name))
            changePortSettings(port_name , settings);
        else setupNewPort(port, settings);
    }
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
    virtual void closeAllPorts() override
    {
        for(auto&& port : set_ports_)
            port->closePort();
    }
    virtual QStringList restorePorts() const override
    {
        return set_ports_.keys();
    }
    virtual void sendData(QString port_name, QByteArray data) override
    {
        output_buffers_[port_name]->appendBytes(data);
    }
};

}
