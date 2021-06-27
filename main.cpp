#include "main.h"
#include <algorithm>

Receiver::Receiver():
    port_list_{QSerialPortInfo::availablePorts()},
    device_{new QSerialPort}
{
    qDebug() << "Looking for devices..." ;
    if(!port_list_.empty())
        qDebug() << "Found something!" ;

    if(!port_list_.empty())
    {
        device_->setPort(port_list_.first());
        if(device_->open(QSerialPort::ReadWrite))
        {
            qDebug() << "port name found!";
            device_->setBaudRate(QSerialPort::Baud9600);
            device_->setDataBits(QSerialPort::Data8);
            device_->setParity(QSerialPort::NoParity);
            device_->setStopBits(QSerialPort::OneStop);
            device_->setFlowControl(QSerialPort::NoFlowControl);
        }
    }

    connect(this->device_.data() , SIGNAL(readyRead()) , this , SLOT(readFromPort() ) );

}

void Receiver::writeOutPortInfo() const
{
    for(auto&& port_info : port_list_)
        qDebug() << port_info.portName() << " " << port_info.description() << " " << port_info.manufacturer() << " " << port_info.systemLocation();
}

QVector<int> getUint(const QByteArray& bytes)
{
    QVector<int> bytes_numeric {};
    bytes_numeric.resize(bytes.size());

    for(int i{0}; i < bytes.size() ; ++i)
        bytes_numeric[i] = bytes.at(i);

    return bytes_numeric;
}

void Receiver::readFromPort() const
{
    for(auto&& byte : getUint(this->device_->readAll()))
        qDebug() << byte;
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

    Receiver rec{};

    rec.writeOutPortInfo();

    return app.exec();
}

