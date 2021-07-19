#pragma once

#include <QStateMachine>
#include <QState>
#include "../backend/PortOperator.hpp"
#include <QDebug>

class Printer:
        public QObject
{
    Q_OBJECT;
public:
    port::ByteBuffer* buf_;
    virtual ~Printer(){};
public slots:
    void print();
};


class Program
{
public:
    Program();
};

class Terminal
{

};

class Tests
{

};

class Settings
{

};
