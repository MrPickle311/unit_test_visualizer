#pragma once

#include "Port_UnitTests.hpp"

class PortInputOperatorTEST:
        public DebugLogger
{
protected:
    port::DataHandler         handler_;
    port::PortScanner         scanner_;
    port::PortInputOperator   operator_;
public:
    PortInputOperatorTEST();
    void selectPort(uint port_nmbr);
    void showPorts() const;
    void waitAndShowArrivingData();
    void openPort();
    void runIntegrationTest();
};
