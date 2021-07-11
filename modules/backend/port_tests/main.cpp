#include "Port_IntegrationTests.hpp"
#include "Port_UnitTests.hpp"
#include <QCoreApplication>
#include <QEventLoop>

void runAllUnitTests(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}

void PortIntegrationTests()
{
    PortInputOperatorTEST operation_integration_test;
    operation_integration_test.runIntegrationTest();
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    runAllUnitTests(argc, argv);

    PortIntegrationTests();

    qDebug() << "Test done , now you can exit...";

    return app.exec();
}
