#include "ErrorReporter.hpp"

ErrorReporter::ErrorReporter(QObject *parent) : QObject(parent)
{}

void ErrorReporter::errorService(std::logic_error error)
{
    QMutexLocker lock{&mutex_};

    QString error_msg{error.what()};

    emit propagateError(error_msg);
}

