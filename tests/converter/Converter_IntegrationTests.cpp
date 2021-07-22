#include "Converter_IntegrationTests.hpp"
#include <QObject>

Converter_IntegrationTests::Converter_IntegrationTests():
    parser_connection_{new QMetaObject::Connection},
    converter_{data_result_}
{
    operator_.setInputByteBuffer(&input_buffer_);
    operator_.setOutputByteBuffer(&output_buffer_);
    operator_.changeSettings(port::StandardSettings::getStandardSettings(port::StandardSetting::StandardSetting57600));

    operator_.changePort(scanner_.getPortByName("COM4"));//in my PC its COM4

    configureParser();
}

template<typename  T>
using Ptr = QSharedPointer<T>;

void Converter_IntegrationTests::configureParser()
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

    makeConnections();
}

void Converter_IntegrationTests::makeConnections()
{
    *parser_connection_ = QObject::connect(&input_buffer_ ,&port::ByteBuffer::bytesArrived,[&]([[maybe_unused]] size_t count ){
        QObject::disconnect(*parser_connection_);
        qDebug() << "Invoked!";
        parser_.startProcessing();
        showResults();
    } );
}

void Converter_IntegrationTests::showResults()
{
    data_result_ = *parser_.getPackage();
    result_ = converter_.getConvertedTransaction();
    printTests(result_);
}

void Converter_IntegrationTests::run()
{
    if(operator_.openPort())
    {
        qDebug() << "Port has been opened successfully";
        output_buffer_.appendByte(51);
    }
}

void printTests(Transaction transaction)
{
    for(auto&& test_case : transaction.cases_)
    {
        qDebug() << test_case.test_case_name_;
        for(auto&& unit_test : test_case.tests_)
        {
            qDebug() << "--- " << unit_test.name_;
            qDebug() << "-------- " << unit_test.type_descriptor_;
            qDebug() << "-------- " << unit_test.current_value_;
            if(unit_test.expecteted_value_.isEmpty())
            {
                qDebug() << "-------- " << unit_test.lower_value_;
                qDebug() << "-------- " << unit_test.upper_value_;
            }
            else qDebug() << "-------- " << unit_test.expecteted_value_;

            qDebug() << "-------- " << unit_test.test_result_;
        }
    }
}
