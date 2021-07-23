#pragma once

#include <QStateMachine>
#include <QState>
#include "SettingsBridge.hpp"
#include "TerminalBridge.hpp"
#include"TestsBridge.hpp"
#include <QDebug>
#include <QQmlApplicationEngine>

namespace bridge
{

template<typename BufferType,
         typename PortOperatortType,
         typename TrasnactionPackageType,
         typename ParserType,
         typename ConverterType>
class Program
{
    using TestsType = Tests<BufferType,PortOperatortType,TrasnactionPackageType,ParserType,ConverterType>;
    using TerminalType = Terminal<BufferType,PortOperatortType>;
private:
    Scanner          scanner_;
    TerminalSettings terminal_settings_;
    TestsSettings    tests_settings_;
    TerminalType     terminal_;
    TestsType        tests_;
private:
    void makeConnections()
    {

    }
public:
    Program()
    {
        //registerTypes();
        makeConnections();
    }
};

}
