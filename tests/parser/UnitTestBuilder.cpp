#include "UnitTestBuilder.hpp"

void BufferStorage::setBuffer(port::ByteBuffer* newBuffer)
{
    buffer_ = newBuffer;
}

void ByteSender::sendByte(byte_t code)
{
    buffer_->appendByte(code);
}

void ByteSender::sendBytes(QByteArray bytes)
{
    buffer_->appendBytes(bytes);
}

void AbstractUnitTestInjecter::injectName(const QByteArray& name)
{
    sendByte(parser::UnitTestCommand::SENDING_NAME);
    sendBytes(name);
    sendByte('\0');
}

void AbstractUnitTestInjecter::injectType(const QByteArray& type)
{
    sendByte(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR);
    sendByte(type.data()[0]);
}

void AbstractUnitTestInjecter::injectCurrentValue(const QByteArray& current_value)
{
    sendByte(parser::UnitTestCommand::SENDING_CURRENT_VALUE);
    sendBytes(current_value);
}

void AbstractUnitTestInjecter::injectTestResult(const QByteArray& result)
{
    sendByte(parser::UnitTestCommand::SENDING_TEST_RESULT);
    sendBytes(result);
}

void AbstractUnitTestInjecter::injectEnd()
{
    buffer_->appendByte(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT);
}

void AbstractUnitTestInjecter::inject(const QSharedPointer<UnitTestDataPackage>& unit_test)
{
    injectType(unit_test->getDescriptor());
    injectName(unit_test->getName());
    injectCurrentValue(unit_test->getCurrentValue());
    injectValues(unit_test);
    injectTestResult(unit_test->getResult());
    injectEnd();
}

///

void UnitTestInjecter::injectExpectedValue(const QByteArray& expected_values)
{
    sendByte(parser::UnitTestCommand::SENDING_EXPECTED_VALUE);
    sendBytes(expected_values);
}

void UnitTestInjecter::injectValues(const QSharedPointer<UnitTestDataPackage>& unit_test)
{
    injectExpectedValue(unit_test->getExpectedValue());
}

///

void RangeUnitTestInjecter::injectLowerValue(const QByteArray& lower_value)
{
    sendByte(parser::UnitTestCommand::SENDING_LOWER_VALUE);
    sendBytes(lower_value);
}

void RangeUnitTestInjecter::injectUpperValue(const QByteArray& upper_value)
{
    sendByte(parser::UnitTestCommand::SENDING_UPPER_VALUE);
    sendBytes(upper_value);
}

void RangeUnitTestInjecter::injectValues(const QSharedPointer<UnitTestDataPackage>& unit_test)
{
    injectLowerValue(unit_test->getLowerValue());
    injectUpperValue(unit_test->getUpperValue());
}


///

void TestCaseInjecter::injectCaseName(const QByteArray& test_case_name)
{
    sendBytes(test_case_name);
    sendByte('\0');
}

void TestCaseInjecter::inject(const QSharedPointer<TestCaseDataPackage>& test_case)
{
    injectCaseName(test_case->getTestCaseName());

    for(auto&& test : test_case->getTests())
    {
        sendByte(parser::TestCaseCommand::SENDING_UNIT_TEST_RESULT);
        if(test->getExpectedValue().isEmpty())
            range_injecter_.inject(test);
        else unit_injecter_.inject(test);
    }
    sendByte(parser::TestCaseCommand::END_SENDING_TEST_CASE);
}

void TestCaseInjecter::setBuffer(port::ByteBuffer* newBuffer)
{
    BufferStorage::setBuffer(newBuffer);
    range_injecter_.setBuffer(newBuffer);
    unit_injecter_.setBuffer(newBuffer);
}

///

void TransactionInjecter::inject(const QSharedPointer<TransactionDataPackage>& test_case)
{
    buffer_->appendByte(parser::GlobalCommand::START);

    for(auto&& test_case : test_case->getCases())
    {
        sendByte(parser::GlobalCommand::SENDING_TEST_CASE);
        case_injecter_.inject(test_case);
    }

    buffer_->appendByte(parser::GlobalCommand::END_ENTIRE_TRANSACTION);
}

void TransactionInjecter::setBuffer(port::ByteBuffer* newBuffer)
{
    BufferStorage::setBuffer(newBuffer);
    case_injecter_.setBuffer(newBuffer);
}
