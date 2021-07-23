#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <modules/backend/PortOperator.hpp>
#include <modules/bridge/Bridge.hpp>
#include <modules/bridge/SettingsBridge.hpp>
#include <modules/bridge/TerminalBridge.hpp>
#include <QQuickWindow>
#include <modules/backend/Converter.hpp>
#include <modules/bridge/TestsBridge.hpp>

#ifdef MAIN_PROGRAM

using MainProgram = bridge::Program<port::ByteBuffer, port::BufferedPortFlowOperator ,
                                   TransactionDataPackage , parser::GlobalParser , Converter>;


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif


    using namespace bridge;

    QGuiApplication app(argc, argv);

    MainProgram program;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/script/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    port::ByteBuffer buffer;

    return app.exec();
}

#endif
