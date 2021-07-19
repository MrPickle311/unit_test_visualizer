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
    explicit SingletonInterface(QObject* parent = nullptr);
    int getSomeProperty() const;
    void setSomeProperty(int newSomeProperty);

    Q_INVOKABLE void changeProperty();
signals:
    void somePropertyChanged();
};



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
