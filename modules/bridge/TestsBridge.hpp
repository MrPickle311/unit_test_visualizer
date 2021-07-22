#pragma once

#include <QObject>
#include "../backend/PortOperator.hpp"
#include "../backend/Converter.hpp"
#include <QDebug>

class TestsSettingsBridge:
        public QObject
{
    Q_OBJECT;

    Q_PROPERTY(QSerialPort::BaudRate baudRate READ getBaudRate WRITE setBaudRate NOTIFY baudRateChanged);
    Q_PROPERTY(QStringList portNames READ getPortNames  WRITE setPortNames  NOTIFY portNamesChanged);

private:
    QSerialPort::BaudRate baudRate;
    port::PortScanner     scanner_;
public:
    QStringList portNames;

public:
    Q_INVOKABLE void sendSettings(QString port_name);

    const QSerialPort::BaudRate& getBaudRate() const;
    Q_INVOKABLE void setBaudRate(const QSerialPort::BaudRate& newBaudRate);

    const QStringList& getPortNames() const;
    void setPortNames(const QStringList& newPortNames);

    Q_INVOKABLE void scanPorts();

signals:
    void settingsApplied(QSerialPortInfo port , port::PortFlowSettings settings);
    void baudRateChanged();
    void portNamesChanged();
};

class TestsBridge : public QObject
{
    Q_OBJECT
private:
    QSharedPointer<QMetaObject::Connection> parser_connection_;
    void configureParser();
    void makeConnections();
protected:
    port::PortScanner               scanner_;
    port::ByteBuffer                input_buffer_;
    port::ByteBuffer                output_buffer_;
    port::BufferedPortFlowOperator  operator_;
    TransactionDataPackage          data_result_;
    parser::GlobalParser            parser_;
    Converter                       converter_;
    Transaction                     result_;
public:
    explicit TestsBridge(QObject *parent = nullptr);
    Q_INVOKABLE void applySettings(QSerialPortInfo port , port::PortFlowSettings settings);
    void sendDataToFrontend();
    Q_INVOKABLE void run();

signals:
    void sendTestCase(QString test_case_name);
    void sendUnitTest(int test_case_nmbr ,  UnitTest unit_test, bool is_range_test);
};

