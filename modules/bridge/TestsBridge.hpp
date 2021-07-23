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
    virtual void applySettings(QSerialPortInfo port , port::PortFlowSettings settings) = 0;
    virtual void run() = 0;
signals:
    void sendTestCase(QString test_case_name);
    void sendUnitTest(int test_case_nmbr ,  UnitTest unit_test, bool is_range_test);
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
    Transaction             result_;
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

        Ptr<parser::EndParser> end {Ptr<parser::EndParser>::create()};

        Ptr<parser::TestCaseParser> case_parser {Ptr<parser::TestCaseParser>::create()};

        parser_.addChild(parser::GlobalCommand::START ,                 Ptr<parser::GlobalStartParser>::create());
        parser_.addChild(parser::GlobalCommand::SENDING_TEST_CASE,      case_parser);
        parser_.addChild(parser::GlobalCommand::END_ENTIRE_TRANSACTION, end);

        Ptr<parser::UnitTestParser> unit_parser{Ptr<parser::UnitTestParser>::create()};

        case_parser->addChild(parser::TestCaseCommand::SENDING_UNIT_TEST_RESULT , unit_parser);
        case_parser->addChild(parser::TestCaseCommand::END_SENDING_TEST_CASE ,    end);

        unit_parser->addChild(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR ,      Ptr<parser::TypeDescriptorParser>::create());
        unit_parser->addChild(parser::UnitTestCommand::SENDING_NAME ,                 Ptr<parser::NameParser>::create());
        unit_parser->addChild(parser::UnitTestCommand::SENDING_CURRENT_VALUE ,        Ptr<parser::CurrentValueParser>::create());
        unit_parser->addChild(parser::UnitTestCommand::SENDING_EXPECTED_VALUE ,       Ptr<parser::ExpectedValueParser>::create());
        unit_parser->addChild(parser::UnitTestCommand::SENDING_TEST_RESULT ,          Ptr<parser::TestResultParser>::create());
        unit_parser->addChild(parser::UnitTestCommand::SENDING_UPPER_VALUE ,          Ptr<parser::UpperValueParser>::create());
        unit_parser->addChild(parser::UnitTestCommand::SENDING_LOWER_VALUE ,          Ptr<parser::LowerValueParser>::create());
        unit_parser->addChild(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT , end);
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
        *parser_connection_ = QObject::connect(&input_buffer_ ,&port::ByteBuffer::bytesArrived,
                                               this , &Tests::process  );
    }
public:
    virtual void applySettings(QSerialPortInfo port, port::PortFlowSettings settings) override
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
