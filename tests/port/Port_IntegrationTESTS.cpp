#include "Port_IntegrationTests.hpp"
#include <QThread>


PortInputOperatorTEST::PortInputOperatorTEST():
    scanner_{},
    operator_{}
{
    operator_.setInputByteBuffer(&input_buffer_);
    operator_.setOutputByteBuffer(&output_buffer_);
    operator_.changeSettings(port::StandardSettings::getStandardSettings(port::StandardSetting::StandardSetting9600));
}

void PortInputOperatorTEST::selectPort(uint port_nmbr)
{
    operator_.changePort(scanner_.getSelectedPort(port_nmbr));
}

void PortInputOperatorTEST::showPorts() const
{
    sendListDataToDebug(scanner_.getCompletePortData());
}

void PortInputOperatorTEST::waitAndShowArrivingData()
{
    SignalChecker input_checker;

    QObject::connect(&operator_ ,&port::PortFlowOperator::dataArrived,
                     &input_checker , &SignalChecker::checkEvent);

    output_buffer_.appendBytes(QByteArray{"gg"});

    input_checker.waitAndProcessObjectEvent();

    EXPECT_FALSE(input_buffer_.isEmpty());
    qDebug() << input_buffer_.getAllBytes();
}

void PortInputOperatorTEST::openPort()
{
    EXPECT_TRUE(operator_.openPort());
}

void PortInputOperatorTEST::runIntegrationTest()
{
    EXPECT_NO_THROW(showPorts());
    EXPECT_NO_FATAL_FAILURE(selectPort(0));
    EXPECT_NO_THROW(operator_.openPort());
    EXPECT_NO_THROW(waitAndShowArrivingData());
}
