#pragma once

#include "SettingsBridge.hpp"
#include "TerminalBridge.hpp"
#include "TestsBridge.hpp"
#include <QDebug>
#include <QQmlApplicationEngine>
#include "../backend/PortSettings.hpp"
#include "../backend/ConvertedDataTypes.hpp"
#include "../backend/PortScanner.hpp"

namespace bridge
{

template<typename BufferType,
         typename PortOperatortType>
class Program
{
    using TestsType = Tests<BufferType,PortOperatortType>;
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
    Program()://injection via constructor
        scanner_{QSharedPointer<backend::PortScanner>::create()}
    {
        registerTypes();
        makeConnections();
    }
};

}
