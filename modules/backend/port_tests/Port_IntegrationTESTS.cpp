#include "Port_IntegrationTests.hpp"

PortInputOperatorTEST::PortInputOperatorTEST():
    scanner_{},
    operator_{}
{
    operator_.setDataHandler(&handler_);
    operator_.changeSettings(port::StandardSettings::getStandardSettings(port::StandardSetting::StandardSetting9600));
}

void PortInputOperatorTEST::selectPort(uint port_nmbr)
{
    operator_.changePort(scanner_.getSelectedPort(port_nmbr));
}

void PortInputOperatorTEST::showPorts() const
{
    sendListDataToDebug(scanner_.getCompletePortData());
    //for(auto&& port: scanner_.getCompletePortData())
    //    qDebug() << port;
}

void PortInputOperatorTEST::waitAndShowArrivingData()
{
    QByteArray array {handler_.getAllReceivedBytes()};
    EXPECT_NE(array.size(), NO_BYTES);
}

void PortInputOperatorTEST::openPort()
{
    EXPECT_TRUE(operator_.openPort());
}

void PortInputOperatorTEST::runIntegrationTest()
{
    EXPECT_NO_THROW(showPorts());
    EXPECT_NO_FATAL_FAILURE(selectPort(1));
    EXPECT_NO_THROW(operator_.openPort());
    EXPECT_NO_THROW(waitAndShowArrivingData());
}
