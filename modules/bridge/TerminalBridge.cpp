#include "TerminalBridge.hpp"


void TerminalBridge::changePortSettings(const QString& port_name, const port::PortFlowSettings& settings)
{
    if(set_ports_[port_name]->isOpen())
        set_ports_[port_name]->closePort();
    set_ports_[port_name]->changeSettings(settings);
}

void TerminalBridge::setupNewPort(const QSerialPortInfo& port, const port::PortFlowSettings& settings)
{
    QString port_name{port.portName()};

    set_ports_[port_name].reset(new port::BufferedPortFlowOperator{settings , port , this});
    output_buffers_[port_name].reset(new port::ByteBuffer{this});
    input_buffers_[port_name].reset(new port::ByteBuffer{this});

    set_ports_[port_name]->setOutputByteBuffer(output_buffers_[port_name].get());
    set_ports_[port_name]->setInputByteBuffer(input_buffers_[port_name].get());
}

void TerminalBridge::applySettings(QSerialPortInfo port, port::PortFlowSettings settings)
{
    QString port_name{port.portName()};

    if(set_ports_.contains(port_name))
        changePortSettings(port_name , settings);
    else setupNewPort(port, settings);
}
