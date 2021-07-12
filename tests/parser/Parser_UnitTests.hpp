#pragma once

#include <common.hpp>
#include <Parser.hpp>
#include <PortOperator.hpp>

//parser erases all commands and splits incoming data into
//certain data
//converter converts each package to full text which can be
//send to frontend-qml

class LocalParser_UnitTests:
        public ::testing::Test
{
protected:
    LocalParser parser_;
    port::ByteBuffer buffer_;
public:
    LocalParser_UnitTests();
    void appendCode(uint8_t code);
};

class StateMachineTest
{

};
