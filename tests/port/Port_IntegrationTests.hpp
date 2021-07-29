#pragma once

#include "Port_UnitTests.hpp"
#include "PortScanner.hpp"

class PortInputOperatorTEST:
        public DebugLogger
{
protected:
    backend::ByteBuffer                 input_buffer_;
    backend::ByteBuffer                 output_buffer_;
    backend::PortScanner                scanner_;
    backend::BufferedPortFlowOperator   operator_;
public:
    PortInputOperatorTEST();
    void selectPort(uint port_nmbr);
    void showPorts() const;
    void waitAndShowArrivingData();
    void openPort();
    void runIntegrationTest();
};
