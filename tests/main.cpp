#include <port/Port_IntegrationTests.hpp>
#include <port/Port_UnitTests.hpp>
#include <QCoreApplication>
#include <parser/Parser_UnitTests.hpp>
#include <converter/Converter_IntegrationTests.hpp>

#define ON 1
#define OFF 0

#define PORT_INTEGRATION_TESTS OFF
#define CONVERTER_INTEGRATION_TESTS ON

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

#if PORT_INTEGRATION_TESTS
    PortIntegrationTests();
#endif

#if CONVERTER_INTEGRATION_TESTS
    Converter_IntegrationTests tests;//plug and play
    tests.run();
#endif

    qDebug() << "Test done , now you can exit...";

    return app.exec();
}


