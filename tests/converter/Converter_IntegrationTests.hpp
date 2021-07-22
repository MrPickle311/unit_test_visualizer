#pragma once
#include <common.hpp>
#include <PortOperator.hpp>
#include <Parser.hpp>
#include <Converter.hpp>

void printTests(Transaction transaction);

class Converter_IntegrationTests
{
private:
    QSharedPointer<QMetaObject::Connection> parser_connection_;
protected:
    port::PortScanner                      scanner_;
    port::ByteBuffer                       input_buffer_;
    port::ByteBuffer                       output_buffer_;
    port::BufferedPortFlowOperator         operator_;
    TransactionDataPackage                 data_result_;
    parser::GlobalParser                   parser_;
    Converter                              converter_;
    Transaction                            result_;
protected:
    void configureParser();
    void makeConnections();
    void showResults();
public:
    Converter_IntegrationTests() ;
    void run();
};

