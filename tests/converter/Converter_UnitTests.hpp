#pragma once

#include <common.hpp>
#include <parser/Parser_UnitTests.hpp>
#include <Converter.hpp>

class ConverterTest:
        public ::testing::Test
{
protected:
    TransactionDataPackage transaction_pack_;
    Converter              converter_;
    TestPackageFactory     factory_;
    Transaction            result_;
public:
    ConverterTest();
    void run();
};
