#include "ErrorReporter.hpp"
#include <QtDebug>

ErrorReporter::ErrorReporter(QObject *parent) : QObject(parent)
{}

void ErrorReporter::errorService(std::logic_error error)
{
    QMutexLocker lock{&mutex_};

    qDebug() << "cought exc";
    QString error_msg{error.what()};

    qDebug() << error_msg;

    emit propagateError(error_msg);
}

