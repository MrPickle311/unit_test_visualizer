#pragma once

#include <QObject>
#include "../backend/PortOperator.hpp"

class TerminalBridge
        : public QObject
{
    Q_OBJECT
public:
    explicit TerminalBridge(QObject *parent = nullptr);

signals:

};

