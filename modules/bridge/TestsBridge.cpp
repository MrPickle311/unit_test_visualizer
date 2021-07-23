#include "TestsBridge.hpp"

TestsBridge::TestsBridge(QObject* parent):
    QObject{parent},
    parser_connection_{new QMetaObject::Connection},
    converter_{data_result_}
{
    operator_.setInputByteBuffer(&input_buffer_);
    operator_.setOutputByteBuffer(&output_buffer_);
    configureParser();
}

void TestsBridge::applySettings(QSerialPortInfo port, port::PortFlowSettings settings)
{
    qDebug() << " QSerialPortInfo name : " << port.portName() << " " << settings.baudRate()  << " " << settings.dataBits() << " " << settings.parity() << " " << settings.stopBits();

    QString port_name{port.portName()};

    operator_.changeSettings(settings);
    operator_.changePort(port);

    qDebug() << "TestsBridge New port is set!";
}

void TestsBridge::sendDataToFrontend()
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

template<typename  T>
using Ptr = QSharedPointer<T>;

void TestsBridge::configureParser()
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

    //makeConnections();
}

void TestsBridge::makeConnections()
{
    *parser_connection_ = QObject::connect(&input_buffer_ ,&port::ByteBuffer::bytesArrived,[&]([[maybe_unused]] size_t count ){
        QObject::disconnect(*parser_connection_);
        qDebug() << "Invoked!";
        parser_.startProcessing();
        data_result_ = *parser_.getPackage();
        result_ = converter_.getConvertedTransaction();
        operator_.closePort();
        sendDataToFrontend();
    } );
}

void TestsBridge::run()
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

