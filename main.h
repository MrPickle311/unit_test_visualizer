#pragma once

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QString>
#include <QList>

class Receiver: public QObject
{
    Q_OBJECT;
private:
    QList<QSerialPortInfo> port_list_;
    QSharedPointer<QSerialPort> device_;
public:
    Receiver();
    void writeOutPortInfo() const;

public slots:

    void readFromPort() const;
};
