#pragma once

#include <QObject>
#include <QMutex>

class ErrorReporter : public QObject
{
    Q_OBJECT
private:
    QMutex mutex_;
public:
    explicit ErrorReporter(QObject *parent = nullptr);
public slots:
    void errorService(std::logic_error error);
signals:
    void propagateError(QString what);
};

