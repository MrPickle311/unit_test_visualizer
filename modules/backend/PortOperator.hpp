#pragma once

#include "PortSettings.hpp"

namespace port
{

//one operator per one port
class PortOperator : public QObject//it only opens a port , nothing else
{
    Q_OBJECT;
protected:
    QSerialPort           current_port_;
    QSerialPortInfo       current_port_info_;
    QSerialPort::OpenMode open_mode_;
protected:
    virtual void openHook(){};
    virtual void closeHook(){};
public:
    PortOperator(QSerialPort::OpenMode open_mode, QObject* parent);
public slots:
    void changePort(QSerialPortInfo port);
    void changeSettings(PortFlowSettings settings);//only copy ,so nothing unexpected will happen
    void closePort();
    bool openPort();
};

//class PortOutputOperator : public PortOperator
//{
//    Q_OBJECT;
//    //i will implement it later
//};

class PortInputOperator : public PortOperator
{
    Q_OBJECT;
private:
    DataHandler* current_data_handler_;
private:
    void makeConnections();
public:
    PortInputOperator(QObject* parent = nullptr);
    PortInputOperator(PortFlowSettings settings ,
                      QSerialPortInfo  port     ,
                      DataHandler* data_handler ,
                      QObject* parent = nullptr);
public:
    void setDataHandler(DataHandler* handler);
public slots:
    void sendDataFromPortToHandler(); //invoked automatically
};

}
