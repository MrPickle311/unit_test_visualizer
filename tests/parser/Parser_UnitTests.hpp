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
    QSharedPointer<UnitTestDataPackage> allocPackage();
public:
    QSharedPointer<UnitTestDataPackage> createBoolUnitTest();
    QSharedPointer<UnitTestDataPackage> createUint32UnitTest();
    QSharedPointer<UnitTestDataPackage> createBitUnitTest();
    QSharedPointer<UnitTestDataPackage> createInt64UnitTest();
    QSharedPointer<UnitTestDataPackage> createPtrUnitTest();
    QSharedPointer<UnitTestDataPackage> createInt16RangeUnitTest();
};

class ParserTests:
        public ::testing::Test
{
protected:
    QSharedPointer<TransactionDataPackage> result_;
    QSharedPointer<parser::GlobalParser>   root_;
    QSharedPointer<port::ByteBuffer>       buffer_;
    TransactionInjecter                    injecter_;
    TestPackageFactory                         factory_;
public:
    ParserTests();
    void insertDataAndRun(QSharedPointer<TransactionDataPackage> transaction);
};

