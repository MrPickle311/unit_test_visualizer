#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QString>
#include <QList>


void test()
{
    qDebug() << "Looking for devices...\n" ;

    QSerialPort port;
    port.setBaudRate(QSerialPort::Baud9600);

    QList<QSerialPortInfo> port_list{QSerialPortInfo::availablePorts()};


    for(auto&& port_info : port_list)
           qDebug() << port_info.portName() << " " << port_info.description();

}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

   qDebug() << "Xd";

    test();

    return app.exec();
}
