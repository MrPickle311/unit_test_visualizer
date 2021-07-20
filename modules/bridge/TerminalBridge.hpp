#pragma once

#include <QObject>
#include "../backend/PortOperator.hpp"
#include <utility>

class TerminalBridge
        : public QObject
{
    Q_OBJECT

    using BufferPtr = QScopedPointer< port::ByteBuffer >;
    using PortOperatorPtr = QScopedPointer<port::BufferedPortFlowOperator>;

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
signals:

};

