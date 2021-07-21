#pragma once

#include <QObject>
#include "../backend/PortOperator.hpp"
#include "../backend/Converter.hpp"
#include <QDebug>

void printTests(Transaction transaction)
{
    for(auto&& test_case : transaction.cases_)
    {
        qDebug() << test_case.test_case_name_;
        for(auto&& unit_test : test_case.tests_)
        {
            qDebug() << "-- " << unit_test.name_;
            qDebug() << "--- " << unit_test.type_descriptor_;
            qDebug() << "--- " << unit_test.current_value_;
            if(unit_test.expecteted_value_.isEmpty())
            {
                qDebug() << "--- " << unit_test.lower_value_;
                qDebug() << "--- " << unit_test.upper_value_;
            }
            else qDebug() << "--- " << unit_test.expecteted_value_;

            qDebug() << "--- " << unit_test.test_result_;
        }
    }
}

class TestsBridge : public QObject
{
    Q_OBJECT
public:
    explicit TestsBridge(QObject *parent = nullptr);

signals:

};

