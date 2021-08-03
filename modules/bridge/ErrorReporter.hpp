#pragma once

#include <QObject>
#include <QMutex>

namespace bridge
{

/// @brief Object of this class propagates errors from C++ to QML
///
/// This type is threadsafe
class ErrorReporter : public QObject
{
    Q_OBJECT
private:
    QMutex mutex_;
public:
    /// Default constructor
    /// @param parent QObject parent pointer
    explicit ErrorReporter(QObject *parent = nullptr);
public slots:
    /// This method converts error object , converts it to string
    /// version and resends it to QML
    /// @param error Error to be converted and propagated
    void errorService(std::logic_error error);
signals:
    /// This signal sends stringlized error to QML
    /// @param what String error to send
    void propagateError(QString what);
};

}
