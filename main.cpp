#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <modules/backend/PortOperator.hpp>
#include <modules/bridge/Bridge.hpp>
#include <modules/backend/PortScanner.hpp>
#include <modules/backend/Converter.hpp>
#include <modules/backend/Parser.hpp>

#ifdef MAIN_PROGRAM

using MainProgram = bridge::Program<backend::ByteBuffer,
                                    backend::BufferedPortFlowOperator>;


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif


    using namespace bridge;

    QGuiApplication app(argc, argv);

    MainProgram program(
                QSharedPointer<backend::PortScanner>::create(),
                QSharedPointer<backend::Converter>::create()  ,
                backend::ReadyParsers::getValue(backend::ParserImplementations::FirstImplementation)
    );

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/script/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    backend::ByteBuffer buffer;

    return app.exec();
}

#endif
