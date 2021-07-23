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

    qRegisterMetaType<UnitTest>();
    qmlRegisterType<QSerialPort>("com.myProject", 1, 0, "SerialPort");


    auto term_bridge{QSharedPointer<TerminalBridge>::create()};
    qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"TerminalBridge",term_bridge.get());

    auto tests_bridge {QSharedPointer<TestsBridge>::create()};
    qmlRegisterSingletonInstance<TestsBridge>("Qt.singletons.bridge",1,0,"TestsBridge",tests_bridge.get());

    ///settings binding

    auto terminal_settings{QSharedPointer<bridge::TerminalSettingsBridge>::create()};
    qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"TerminalSettingsBridge",terminal_settings.get());

    auto tests_settings{QSharedPointer<bridge::TestsSettingsBridge>::create()};
    qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"TestsSettingsBridge",tests_settings.get());

    bridge::Scanner scanner{QSharedPointer<port::PortScanner>::create()};
    qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"Scanner", &scanner);


    QObject::connect(&scanner , &bridge::Scanner::portsScanned , terminal_settings.get() , &bridge::SettingsBridge::setPortNames );
    QObject::connect(&scanner , &bridge::Scanner::portsScanned , tests_settings.get() , &bridge::SettingsBridge::setPortNames );

    QObject::connect(terminal_settings.get() , &bridge::SettingsBridge::portRequest , &scanner , &bridge::Scanner::getPortByName );
    QObject::connect(tests_settings.get() , &bridge::SettingsBridge::portRequest , &scanner , &bridge::Scanner::getPortByName );




    QObject::connect(terminal_settings.get(), &bridge::SettingsBridge::settingsApplied ,
                     term_bridge.get(), &TerminalBridge::applySettings );


    QObject::connect(tests_settings.get(), &bridge::TestsSettingsBridge::settingsApplied ,
                     tests_bridge.get(), &TestsBridge::applySettings );

    ///END settings binding




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
