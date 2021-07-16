#pragma once

#include <common.hpp>
#include <PortOperator.hpp>
#include "UnitTestBuilder.hpp"
#include <Parser.hpp>

#define FAILURE 0
#define PASSED  1

class ParserTests:
        public ::testing::Test
{
protected:
    QSharedPointer<TransactionDataPackage> result_;
    QSharedPointer<parser::GlobalParser>   root_;
    QSharedPointer<port::ByteBuffer>       buffer_;
    TransactionInjecter                    injecter_;
protected:
    QSharedPointer<UnitTestDataPackage> allocTest();
public:
    ParserTests();
    QSharedPointer<UnitTestDataPackage> createBoolUnitTest();
    QSharedPointer<UnitTestDataPackage> createUint32UnitTest();
    QSharedPointer<UnitTestDataPackage> createBitUnitTest();
    QSharedPointer<UnitTestDataPackage> createInt64UnitTest();
    QSharedPointer<UnitTestDataPackage> createPtrUnitTest();
    QSharedPointer<UnitTestDataPackage> createInt16RangeUnitTest();
    void insertDataAndRun(QSharedPointer<TransactionDataPackage> transaction);
};

