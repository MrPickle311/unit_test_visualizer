#include "Converter_IntegrationTests.hpp"
#include <QObject>

Converter_IntegrationTests::Converter_IntegrationTests():
    parser_connection_{new QMetaObject::Connection},
    parser_{backend::ReadyParsers::getValue(backend::ParserImplementations::FirstImplementation)},
    converter_{}
{
    operator_.setInputByteBuffer(&input_buffer_);
    operator_.setOutputByteBuffer(&output_buffer_);
    operator_.changeSettings(backend::StandardSettings::getValue(backend::StandardSetting::StandardSetting57600));

    operator_.changePort(scanner_.getPortByName("COM6"));//in my PC its COM4 or COM6

    converter_.setPack(&data_result_);

    configureParser();
    makeConnections();
}

template<typename  T>
using Ptr = QSharedPointer<T>;

void Converter_IntegrationTests::configureParser()
{
    parser_->setBuffer(&input_buffer_);
}

void Converter_IntegrationTests::makeConnections()
{
    *parser_connection_ = QObject::connect(&input_buffer_ ,&backend::ByteBuffer::bytesArrived,[&]([[maybe_unused]] size_t count ){
        QObject::disconnect(*parser_connection_);
        qDebug() << "Invoked!";
        parser_->startProcessing();
        showResults();
    } );
}

void Converter_IntegrationTests::showResults()
{
    data_result_ = *parser_->getPackage();
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

void printTests(backend::Transaction transaction)
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
