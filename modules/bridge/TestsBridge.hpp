#pragma once

#include <QObject>
#include "../backend/PortOperator.hpp"
#include "../backend/Converter.hpp"
#include <QDebug>



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

