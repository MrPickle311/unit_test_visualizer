#pragma once

#include <QObject>
#include "../backend/PortOperator.hpp"
#include "../backend/Converter.hpp"
#include <utility>

class TerminalBridge
        : public QObject
{
    Q_OBJECT;

    using BufferPtr = QSharedPointer< port::ByteBuffer >;
    using PortOperatorPtr = QSharedPointer<port::BufferedPortFlowOperator>;

public:

private:
    QMap<QString , PortOperatorPtr > set_ports_;
    QMap<QString , BufferPtr>        input_buffers_;
    QMap<QString , BufferPtr>        output_buffers_;
private:
    void changePortSettings(const QString& port_name , const port::PortFlowSettings& settings);
    void setupNewPort( const QSerialPortInfo& port , const port::PortFlowSettings& settings);

public slots:
    void applySettings(QSerialPortInfo port , port::PortFlowSettings settings);
    Q_INVOKABLE void openPort(QString port_name);
    Q_INVOKABLE void closeAllPorts();
    Q_INVOKABLE QStringList restorePorts() const;

    Q_INVOKABLE void sendData(QString port_name , QByteArray data);
signals:
    void newPortIsSet(QString port_name);
    void dataArrived(QString port_name ,  QByteArray data);

    void unitTestCaseSent(int nmbr, UnitTest test , bool is_range);
};

