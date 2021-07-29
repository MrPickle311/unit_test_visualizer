#pragma once

#include <common.hpp>
#include <PortOperator.hpp>
#include "UnitTestBuilder.hpp"
#include <Parser.hpp>

#define FAILURE 0
#define PASSED  1

class TestPackageFactory
{
protected:
    QSharedPointer<backend::UnitTestDataPackage> allocPackage();
public:
    QSharedPointer<backend::UnitTestDataPackage> createBoolUnitTest();
    QSharedPointer<backend::UnitTestDataPackage> createUint32UnitTest();
    QSharedPointer<backend::UnitTestDataPackage> createBitUnitTest();
    QSharedPointer<backend::UnitTestDataPackage> createInt64UnitTest();
    QSharedPointer<backend::UnitTestDataPackage> createUint64UnitTest();
    QSharedPointer<backend::UnitTestDataPackage> createPtrUnitTest();
    QSharedPointer<backend::UnitTestDataPackage> createInt16RangeUnitTest();
};

class ParserTests:
        public ::testing::Test
{
protected:
    QSharedPointer<backend::TransactionDataPackage> result_;
    QSharedPointer<backend::RootParser>             root_;
    QSharedPointer<backend::ByteBuffer>             buffer_;
    TransactionInjecter                             injecter_;
    TestPackageFactory                              factory_;
public:
    ParserTests();
    void insertDataAndRun(QSharedPointer<backend::TransactionDataPackage> transaction);
};

//converts lits of bytes to QByteArray
QByteArray bytes(QList<uint8_t> byte_lits);
