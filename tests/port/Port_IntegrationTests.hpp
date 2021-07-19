#pragma once

#include "Port_UnitTests.hpp"

class PortInputOperatorTEST:
        public DebugLogger
{
protected:
    port::ByteBuffer                 input_buffer_;
    port::ByteBuffer                 output_buffer_;
    port::PortScanner                scanner_;
    port::BufferedPortFlowOperator   operator_;
public:
    PortInputOperatorTEST();
    void selectPort(uint port_nmbr);
    void showPorts() const;
    void waitAndShowArrivingData();
    void openPort();
    void runIntegrationTest();
};
