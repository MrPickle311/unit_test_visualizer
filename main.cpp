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

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QScopedPointer<SingletonInterface> singleton{new SingletonInterface};

    QScopedPointer<SettingsBridge> bridge{new SettingsBridge};

    QScopedPointer<TerminalBridge> term_bridge{new TerminalBridge};

    qRegisterMetaType<UnitTest>();

    QObject::connect(bridge.get(), &SettingsBridge::settingsApplied , term_bridge.get(), &TerminalBridge::applySettings );



    qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"SettingsBridge",bridge.get());
    qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"TerminalBridge",term_bridge.get());

    QScopedPointer<TestsSettingsBridge> tests_settings_bridge{new TestsSettingsBridge};

    qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"TestsSettingsBridge",tests_settings_bridge.get());



    QScopedPointer<TestsBridge> tests_bridge {new TestsBridge};

    qmlRegisterSingletonInstance<TestsBridge>("Qt.singletons.bridge",1,0,"TestsBridge",tests_bridge.get());

    QObject::connect(tests_settings_bridge.get(), &TestsSettingsBridge::settingsApplied ,
                     tests_bridge.get(), &TestsBridge::applySettings );


    qmlRegisterType<QSerialPort>("com.myProject", 1, 0, "SerialPort");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/script/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    //port::PortScanner scanner;

    port::ByteBuffer buffer;

    //qDebug() << scanner.getPortNames();

    //port::BufferedPortFlowOperator port_operator;

    //port_operator.setInputByteBuffer(&buffer);
    //port_operator.changeSettings(port::StandardSettings::getStandardSettings(port::StandardSetting::StandardSetting9600));
    //port_operator.changePort(scanner.getSelectedPort(1));
    //
    //port_operator.openPort();

    //Printer printer;
    //printer.buf_ = &buffer;

    //QObject::connect(&buffer , &port::ByteBuffer::bytesArrived , &printer, &Printer::print);



    return app.exec();
}

#endif
