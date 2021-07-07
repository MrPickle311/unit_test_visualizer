#ifndef TST_TEST1_H
#define TST_TEST1_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QList>
#include "../Converter.hpp"
#include <QDebug>

using namespace testing;

TEST(TEst, test1)
{
    QList<int> list;
    list.append(5);

    PortScanner scanner;
    qDebug() << "PORT: " << scanner.getPortNames();

    EXPECT_EQ(1, 1);
    EXPECT_EQ(list.at(0),7);
    ASSERT_THAT(0, Eq(0));
}

#endif // TST_TEST1_H
