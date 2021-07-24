#pragma once

#include <QObject>
#include "../backend/PortOperator.hpp"
#include "../backend/Converter.hpp"
#include <QDebug>

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
         typename PortOperatortType,
         typename TrasnactionPackageType,
         typename ParserType,
         typename ConverterType>
class Tests:
        public TestsBody
{
private:
    BufferType              input_buffer_;
    BufferType              output_buffer_;
    PortOperatortType       operator_;
    TrasnactionPackageType  data_result_;
    ParserType              parser_;
    ConverterType           converter_;
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
        parser_.setBuffer(&input_buffer_);

        Ptr<backend::EndParser> end {Ptr<backend::EndParser>::create()};

        Ptr<backend::TestCaseParser> case_parser {Ptr<backend::TestCaseParser>::create()};

        parser_.addChild(backend::GlobalCommand::START ,                 Ptr<backend::GlobalStartParser>::create());
        parser_.addChild(backend::GlobalCommand::SENDING_TEST_CASE,      case_parser);
        parser_.addChild(backend::GlobalCommand::END_ENTIRE_TRANSACTION, end);

        Ptr<backend::UnitTestParser> unit_parser{Ptr<backend::UnitTestParser>::create()};

        case_parser->addChild(backend::TestCaseCommand::SENDING_UNIT_TEST_RESULT , unit_parser);
        case_parser->addChild(backend::TestCaseCommand::END_SENDING_TEST_CASE ,    end);

        unit_parser->addChild(backend::UnitTestCommand::SENDING_TYPE_DESCRIPTOR ,      Ptr<backend::TypeDescriptorParser>::create());
        unit_parser->addChild(backend::UnitTestCommand::SENDING_NAME ,                 Ptr<backend::NameParser>::create());
        unit_parser->addChild(backend::UnitTestCommand::SENDING_CURRENT_VALUE ,        Ptr<backend::CurrentValueParser>::create());
        unit_parser->addChild(backend::UnitTestCommand::SENDING_EXPECTED_VALUE ,       Ptr<backend::ExpectedValueParser>::create());
        unit_parser->addChild(backend::UnitTestCommand::SENDING_TEST_RESULT ,          Ptr<backend::TestResultParser>::create());
        unit_parser->addChild(backend::UnitTestCommand::SENDING_UPPER_VALUE ,          Ptr<backend::UpperValueParser>::create());
        unit_parser->addChild(backend::UnitTestCommand::SENDING_LOWER_VALUE ,          Ptr<backend::LowerValueParser>::create());
        unit_parser->addChild(backend::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT , end);
    }
    void process([[maybe_unused]] size_t count)
    {
        QObject::disconnect(*parser_connection_);
        qDebug() << "Invoked!";
        parser_.startProcessing();
        data_result_ = *parser_.getPackage();
        result_ = converter_.getConvertedTransaction();
        operator_.closePort();
        sendDataToFrontend();
    }
    void makeConnections()
    {
        *parser_connection_ = QObject::connect(&input_buffer_ ,&backend::ByteBuffer::bytesArrived,
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
        converter_.reset();

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
        converter_{data_result_}
    {
        operator_.setInputByteBuffer(&input_buffer_);
        operator_.setOutputByteBuffer(&output_buffer_);
        configureParser();
    }
};

}
