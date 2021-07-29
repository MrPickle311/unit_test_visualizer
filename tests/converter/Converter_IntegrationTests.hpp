#pragma once
#include <common.hpp>
#include <PortOperator.hpp>
#include <Parser.hpp>
#include <Converter.hpp>
#include <PortScanner.hpp>

void printTests(backend::Transaction transaction);

class Converter_IntegrationTests
{
private:
    QSharedPointer<QMetaObject::Connection> parser_connection_;
protected:
    backend::PortScanner                   scanner_;
    backend::ByteBuffer                    input_buffer_;
    backend::ByteBuffer                    output_buffer_;
    backend::BufferedPortFlowOperator      operator_;
    backend::TransactionDataPackage        data_result_;
    QSharedPointer<backend::RootParser>    parser_;
    backend::Converter                     converter_;
    backend::Transaction                   result_;
protected:
    void configureParser();
    void makeConnections();
    void showResults();
public:
    Converter_IntegrationTests() ;
    void run();
};

