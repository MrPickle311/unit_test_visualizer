#pragma once

#include <QStateMachine>
#include <QState>
#include "SettingsBridge.hpp"
#include "TerminalBridge.hpp"
#include "TestsBridge.hpp"
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
        QObject::connect(&scanner_ , &Scanner::portsScanned , &terminal_settings_ , &Settings::setPortNames );
        QObject::connect(&scanner_ , &Scanner::portsScanned , &tests_settings_ , &Settings::setPortNames );

        QObject::connect(&terminal_settings_ , &Settings::portRequest , &scanner_ , &Scanner::getPortByName );
        QObject::connect(&tests_settings_ , &Settings::portRequest , &scanner_ , &Scanner::getPortByName );

        QObject::connect(&terminal_settings_, &Settings::settingsApplied ,
                         &terminal_, &TerminalBase::applySettings );

        QObject::connect(&tests_settings_, &TestsSettings::settingsApplied ,
                         &tests_, &TestsBody::applySettings );
    }
    void registerTypes()
    {
        qRegisterMetaType<backend::UnitTest>();
        qmlRegisterType<QSerialPort>("com.myProject", 1, 0, "SerialPort");
        qmlRegisterSingletonInstance<TestsType>("Qt.singletons.bridge",1,0,"TestsBridge",&tests_);
        qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"TerminalBridge",&terminal_);
        qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"TerminalSettingsBridge",&terminal_settings_);
        qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"TestsSettingsBridge",&tests_settings_);
        qmlRegisterSingletonInstance("Qt.singletons.bridge",1,0,"Scanner", &scanner_);
    }
public:
    Program():
        scanner_{QSharedPointer<backend::PortScanner>::create()}
    {
        registerTypes();
        makeConnections();
    }
};

}
