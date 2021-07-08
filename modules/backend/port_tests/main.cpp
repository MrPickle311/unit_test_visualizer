#include "Port_IntegrationTests.hpp"
#include "Port_UnitTests.hpp"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    //UNIT TESTS
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    PortInputOperatorTEST operation_integration_test;
    operation_integration_test.runIntegrationTest();

    //INTEGRATION TESTS

    //PortScanner scanner;
    //DataHandler handler;
    //PortInputOperator operator_;
    //
    //operator_.changePort(scanner.getSelectedPort(1));
    //operator_.setDataHandler(&handler);
    //operator_.changeSettings(StandardSettings::getStandardSettings(StandardSetting::StandardSetting9600));
    //qDebug() << operator_.openPort();

    return app.exec();
   // return RUN_ALL_TESTS();
}
