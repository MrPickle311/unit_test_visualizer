#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <modules/backend/PortOperator.hpp>
#include <modules/bridge/Bridge.hpp>
#include <modules/bridge/SettingsBridge.hpp>
#include <QQuickWindow>

#ifdef MAIN_PROGRAM

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QScopedPointer<SingletonInterface> singleton{new SingletonInterface};

    QScopedPointer<SettingsBridge> bridge{new SettingsBridge};

    qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"SettingsBridge",bridge.get());
    qmlRegisterSingletonInstance("Qt.singletons.firstSingleton",1,0,"SingletonInterface",singleton.get());

    qmlRegisterType<QSerialPort>("com.myProject", 1, 0, "SerialPort");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/script/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    port::PortScanner scanner;

    port::ByteBuffer buffer;

    qDebug() << scanner.getPortNames();

    port::BufferedPortFlowOperator port_operator;

    port_operator.setInputByteBuffer(&buffer);
    port_operator.changeSettings(port::StandardSettings::getStandardSettings(port::StandardSetting::StandardSetting9600));
    port_operator.changePort(scanner.getSelectedPort(1));

    port_operator.openPort();

    Printer printer;
    printer.buf_ = &buffer;

    //QObject::connect(&buffer , &port::ByteBuffer::bytesArrived , &printer, &Printer::print);



    return app.exec();
}

#endif
