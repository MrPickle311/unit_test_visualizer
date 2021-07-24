#pragma once

#include <QList>
#include <QString>
#include <QObject>

namespace backend
{

struct UnitTest
{
    Q_GADGET;
    Q_PROPERTY(QString typeDescriptor   MEMBER  type_descriptor_ )
    Q_PROPERTY(QString name             MEMBER  name_            )
    Q_PROPERTY(QString expectetedValue  MEMBER  expecteted_value_)
    Q_PROPERTY(QString currentValue     MEMBER  current_value_   )
    Q_PROPERTY(QString lowerValue       MEMBER  lower_value_     )
    Q_PROPERTY(QString upperValue       MEMBER  upper_value_     )
    Q_PROPERTY(QString testResult       MEMBER  test_result_     )
public:
    QString   type_descriptor_;
    QString   name_;
    QString   expecteted_value_;
    QString   current_value_;
    QString   lower_value_;
    QString   upper_value_;
    QString   test_result_;
};

}

//this macro requires to be placed id the global namespace
Q_DECLARE_METATYPE(backend::UnitTest);


namespace backend
{

struct TestCase
{
    QList<UnitTest> tests_;
    QString         test_case_name_;
};

struct Transaction
{
    QList<TestCase> cases_;
};

}
