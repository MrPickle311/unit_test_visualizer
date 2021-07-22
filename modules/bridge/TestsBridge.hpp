#pragma once

#include <QObject>
#include "../backend/PortOperator.hpp"
#include "../backend/Converter.hpp"
#include <QDebug>



class TestsBridge : public QObject
{
    Q_OBJECT
public:
    explicit TestsBridge(QObject *parent = nullptr);

signals:

};

