#pragma once

#include "Port_UnitTests.hpp"

class PortInputOperatorTEST:
        public DataHandlerTEST,
        public DebugLogger
{
protected:
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
