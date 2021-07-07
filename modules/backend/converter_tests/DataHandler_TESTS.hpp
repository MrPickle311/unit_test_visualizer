#ifndef TST_TEST1_H
#define TST_TEST1_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Converter.hpp"
#include <QDebug>
#include <QObject>
#include <string>

using namespace testing;

class DataHandlerTEST:
        public ::testing::Test
{
protected:
    DataHandler handler_;
public:
    DataHandlerTEST();
    void        appendChars(std::string bytes);
    QByteArray  emptyHandler();
    bool        isHandlerEmpty() const;
    size_t      currentBytesCount() const;
    QByteArray  getSeveralBytes(size_t count);
};

class DataHandler_SignalTester:
        public QObject
{
    Q_OBJECT;
public slots:
    void expectBytes(size_t count);
    void expectEmptyHandler(size_t count);
public:
    ~DataHandler_SignalTester();
};

#endif // TST_TEST1_H
