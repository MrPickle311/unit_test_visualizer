#pragma once

#include <QObject>
#include <QDebug>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSharedPointer>
#include "../backend/PortSettings.hpp"
#include "../backend/ConvertedDataTypes.hpp"
#include "../backend/Interfaces.hpp"
#include "../backend/ParsedDataTypes.hpp"
#include "../backend/Converter.hpp"
#include "../backend/Parser.hpp"

namespace bridge
{

template<typename  T>
using Ptr = QSharedPointer<T>;

class TestsBody:
        public QObject
{
    Q_OBJECT
protected:
    QSharedPointer<QMetaObject::Connection> parser_connection_;
public:
    explicit TestsBody(QObject *parent = nullptr):
        QObject{parent},
        parser_connection_{new QMetaObject::Connection}
    {}
public slots:
    virtual void applySettings(QSerialPortInfo port , backend::PortFlowSettings settings) = 0;
    virtual void run() = 0;
signals:
    void sendTestCase(QString test_case_name);
    void sendUnitTest(int test_case_nmbr ,  backend::UnitTest unit_test, bool is_range_test);
};

template<typename BufferType,
         typename PortOperatortType>
class Tests:
        public TestsBody
{
private:
    BufferType              input_buffer_;
    BufferType              output_buffer_;
    PortOperatortType       operator_;
    backend::TransactionDataPackage  data_result_;
    Ptr<interface::ParserComponent> parser_;
    Ptr<interface::Converter>           converter_;
    backend::Transaction             result_;
private:
    void sendDataToFrontend()
    {
        bool is_range_test{false};
        for(int i{0} ; i < result_.cases_.size() ; ++i)
        {
            qDebug() << "case sending";
            emit sendTestCase(result_.cases_[i].test_case_name_);

            for(auto&& unit_test : result_.cases_[i].tests_)
            {
                if(unit_test.expecteted_value_.isEmpty())
                    is_range_test = true;

                emit sendUnitTest(i,unit_test, is_range_test);
            }

            is_range_test = false;
        }
    }
    virtual void configureParser()
    {
        parser_->setBuffer(&input_buffer_);
    }
    void process([[maybe_unused]] size_t count)
    {
        QObject::disconnect(*parser_connection_);
        qDebug() << "Invoked!";
        parser_->startProcessing();
        data_result_ = *parser_->getPackage();
        result_ = converter_->getConvertedTransaction();
        operator_.closePort();
        sendDataToFrontend();
    }
    void makeConnections()
    {
        *parser_connection_ = QObject::connect(&input_buffer_ ,&interface::ByteBuffer::bytesArrived,
                                               this , &Tests::process  );
    }
public:
    virtual void applySettings(QSerialPortInfo port, backend::PortFlowSettings settings) override
    {
        qDebug() << " QSerialPortInfo name : " << port.portName() << " " << settings.baudRate()
                 << " " << settings.dataBits() << " " << settings.parity() << " " << settings.stopBits();

        QString port_name{port.portName()};

        operator_.changeSettings(settings);
        operator_.changePort(port);

        qDebug() << "TestsBridge New port is set!";
    }
    virtual void run() override
    {
        data_result_.clear();
        result_.cases_.clear();
        converter_->reset();

        if(result_.cases_.isEmpty())
            qDebug() << "Is empty!";

        makeConnections();

        if(operator_.openPort())
        {
            qDebug() << "Port has been opened successfully";
            output_buffer_.appendByte(51);
        }
        else qDebug() << operator_.getError();


    }
    Tests(QObject* parent = nullptr):
        TestsBody{parent},
        converter_{QSharedPointer<backend::Converter>::create(data_result_) },
        parser_{backend::ReadyParsers::getValue(backend::ParserImplementations::FirstImplementation)}
    {
        operator_.setInputByteBuffer(&input_buffer_);
        operator_.setOutputByteBuffer(&output_buffer_);
        configureParser();
    }
    //void setParser (Ptr<interface::ParserComponent> newParser)
    //{
    //    parser_ = newParser;
    //}
};

}
