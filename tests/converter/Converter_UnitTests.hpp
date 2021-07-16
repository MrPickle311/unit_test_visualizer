#pragma once

#include <common.hpp>
#include "Converter_UnitTests.hpp"
#include <Parser.hpp>
#include <Converter.hpp>
#include <parser/Parser_UnitTests.hpp>

TEST_F(ConverterTests , LogicTest)
{
    Transaction transaction;

    TransactionDataPackage parsed_package;

    Converter converter;
    converter.setTarget(parsed_package);

    transaction = converter.getConverterTransaction();



}
