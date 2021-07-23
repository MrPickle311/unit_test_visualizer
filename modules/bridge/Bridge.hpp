#pragma once

#include <QStateMachine>
#include <QState>
#include "SettingsBridge.hpp"
#include <QDebug>

namespace bridge
{

class Program
{
private:
    Scanner scanner_;
    TerminalSettings terminal_settings_;
    TestsSettings    tests_settings_;
    //Terminal         terminal_;
    //Tests            tests_;
private:
    void makeConnections();
public:
    Program();
};

}
