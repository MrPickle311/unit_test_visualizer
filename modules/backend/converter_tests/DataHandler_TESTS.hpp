#ifndef TST_TEST1_H
#define TST_TEST1_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Converter.hpp"
#include <QDebug>
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
    void        showCurrentData() const;
    QByteArray  emptyHandler();
    bool        isHandlerEmpty() const;
    size_t      currentBytesCount() const;
    QByteArray  getSeveralBytes(size_t count);
};

#endif // TST_TEST1_H
