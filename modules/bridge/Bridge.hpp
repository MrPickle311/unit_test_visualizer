#pragma once

#include <QStateMachine>
#include <QState>
#include "../backend/PortOperator.hpp"
#include <QDebug>

class SingletonInterface:
        public QObject
{
    Q_OBJECT;
    Q_PROPERTY(int some_property READ getSomeProperty WRITE setSomeProperty NOTIFY somePropertyChanged);
    int some_property;

public:
    ~SingletonInterface(){}
    explicit SingletonInterface(QObject* parent = nullptr);
    int getSomeProperty() const;
    void setSomeProperty(int newSomeProperty);

    Q_INVOKABLE void changeProperty();
signals:
    void somePropertyChanged();
};

/// i will use it later
//class TerminalInterface:
//        public QObject
//{
//    Q_OBJECT;
//public:
//    virtual ~TerminalInterface(){}
//public slots:
//    virtual QByteArray getAllBytes()  = 0;
//    virtual void       sendAllBytes() = 0;
//signals:
//    void dataArrived();
//};

//class TerminalBridge:
//        public QObject
//{
//    Q_OBJECT;
//private:
//    QMap<Q
//    //TerminalInterface* interface_;
//
//
//public slots:
//
//
//signals:
//};

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
