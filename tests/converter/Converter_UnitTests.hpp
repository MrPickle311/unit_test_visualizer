#pragma once

#include <common.hpp>
#include <parser/Parser_UnitTests.hpp>
#include <Converter.hpp>

class ConverterTest:
        public ::testing::Test
{
protected:
    backend::TransactionDataPackage transaction_pack_;
    backend::Converter              converter_;
    TestPackageFactory              factory_;
    backend::Transaction            result_;
public:
    ConverterTest();
    void run();
};
