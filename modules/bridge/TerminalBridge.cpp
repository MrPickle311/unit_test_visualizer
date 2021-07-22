#include "TerminalBridge.hpp"
#include <QDebug>

void TerminalBridge::changePortSettings(const QString& port_name, const port::PortFlowSettings& settings)
{
    if(set_ports_[port_name]->isOpen())
        set_ports_[port_name]->closePort();
    set_ports_[port_name]->changeSettings(settings);

    qDebug() << "Settings changed!";
}

void TerminalBridge::setupNewPort(const QSerialPortInfo& port, const port::PortFlowSettings& settings)
{
    QString port_name{port.portName()};

    //creating port operator
    set_ports_[port_name].reset(new port::BufferedPortFlowOperator{settings , port , this});

    //creating buffers
    output_buffers_[port_name].reset(new port::ByteBuffer{this});
    input_buffers_[port_name].reset(new port::ByteBuffer{this});

    //binding buffers
    set_ports_[port_name]->setOutputByteBuffer(output_buffers_[port_name].get());
    set_ports_[port_name]->setInputByteBuffer(input_buffers_[port_name].get());

    QObject::connect(input_buffers_[port_name].get() , &port::ByteBuffer::bytesArrived , [this , port_name](size_t count) {
        emit dataArrived(port_name , input_buffers_[port_name]->getAllBytes() );
    } );

    qDebug() << "New port is set!";
    emit newPortIsSet(port_name);
}

void TerminalBridge::applySettings(QSerialPortInfo port, port::PortFlowSettings settings)
{
    qDebug() << " QSerialPortInfo name : " << port.portName();

    QString port_name{port.portName()};

    if(set_ports_.contains(port_name))
        changePortSettings(port_name , settings);
    else setupNewPort(port, settings);

}

void TerminalBridge::openPort(QString port_name)
{
    qDebug() << port_name;
    if(set_ports_[port_name]->openPort())
        qDebug() << port_name + " port opened!";
    else qDebug() << set_ports_[port_name]->getError();
    qDebug() << set_ports_.size();
}

void TerminalBridge::closeAllPorts()
{
    for(auto&& port : set_ports_)
        port->closePort();

    qDebug() << "All ports closed!";
}

QStringList TerminalBridge::restorePorts() const
{
    return set_ports_.keys();
}

void TerminalBridge::resendTest()
{
    UnitTest test;

    test.current_value_ = "434";
    test.type_descriptor_ = "unit8_t";
    test.expecteted_value_ = "434";
    test.name_ = "xd()";
    test.test_result_ = "Passed";

    emit unitTestCaseSent(0,  test , false);
}

void TerminalBridge::sendData(QString port_name, QByteArray data)
{

}


