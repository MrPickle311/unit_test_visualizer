#include "UnitTestBuilder.hpp"

void BufferStorage::setBuffer(port::ByteBuffer* newBuffer)
{
    buffer_ = newBuffer;
}

void AbstractUnitTestDataPackage::injectName()
{
    buffer_->appendByte(parser::UnitTestCommand::SENDING_NAME);
    for(auto&& c : name_)
        buffer_->appendByte(c);
    buffer_->appendByte('\0');
}

void AbstractUnitTestDataPackage::injectType()
{
    buffer_->appendByte(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR);
    buffer_->appendByte(descriptor_);
}

void AbstractUnitTestDataPackage::injectCurrentValue()
{
    buffer_->appendByte(parser::UnitTestCommand::SENDING_CURRENT_VALUE);
    injectMultiValue(current_value_);
}

void AbstractUnitTestDataPackage::injectTestResult()
{
    buffer_->appendByte(parser::UnitTestCommand::SENDING_TEST_RESULT);
    buffer_->appendByte(result_);
}

void AbstractUnitTestDataPackage::injectEnd()
{
    buffer_->appendByte(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT);
}

void AbstractUnitTestDataPackage::injectMultiValue(QList<uint8_t> values)
{
    for(auto&& value : values)
        buffer_->appendByte(value);
}

void AbstractUnitTestDataPackage::inject()
{
    injectType();
    injectName();
    injectCurrentValue();
    injectValues();
    injectTestResult();
    injectEnd();
}

const std::string& AbstractUnitTestDataPackage::getName() const
{
    return name_;
}

void AbstractUnitTestDataPackage::setName(const std::string& newName)
{
    name_ = newName;
}

parser::TypeDescriptor AbstractUnitTestDataPackage::getDescriptor() const
{
    return descriptor_;
}

void AbstractUnitTestDataPackage::setDescriptor(parser::TypeDescriptor newDescriptor)
{
    descriptor_ = newDescriptor;
}

TestResult AbstractUnitTestDataPackage::getResult() const
{
    return result_;
}

void AbstractUnitTestDataPackage::setResult(TestResult newResult)
{
    result_ = newResult;
}

const QList<byte_t>& AbstractUnitTestDataPackage::getCurrentValue() const
{
    return current_value_;
}

void AbstractUnitTestDataPackage::setCurrentValue(const QList<byte_t>& newCurrent_value)
{
    current_value_ = newCurrent_value;
}

///

void UnitTestDataPackage::appendExpectedValue()
{
    buffer_->appendByte(parser::UnitTestCommand::SENDING_EXPECTED_VALUE);
    injectMultiValue(current_value_);
}

void UnitTestDataPackage::injectValues()
{
    appendExpectedValue();
}


const QList<uint8_t>& UnitTestDataPackage::getExpectedValue() const
{
    return expected_value_;
}

void UnitTestDataPackage::setExpectedValue(const QList<uint8_t>& newExpected_value)
{
    expected_value_ = newExpected_value;
}

///

void RangeUnitTestDataPackage::injectLowerValue()
{
    buffer_->appendByte(parser::UnitTestCommand::SENDING_LOWER_VALUE);
    injectMultiValue(lower_value_);
}

void RangeUnitTestDataPackage::injectUpperValue()
{
    buffer_->appendByte(parser::UnitTestCommand::SENDING_UPPER_VALUE);
    injectMultiValue(upper_value_);
}

void RangeUnitTestDataPackage::injectValues()
{
    injectLowerValue();
    injectUpperValue();
}

const QList<uint8_t>& RangeUnitTestDataPackage::getLowerValue() const
{
    return lower_value_;
}

void RangeUnitTestDataPackage::setLowerValue(const QList<uint8_t>& newLower_value)
{
    lower_value_ = newLower_value;
}

const QList<uint8_t>& RangeUnitTestDataPackage::getUpperValue() const
{
    return upper_value_;
}

void RangeUnitTestDataPackage::setUpperValue(const QList<uint8_t>& newUpper_value)
{
    upper_value_ = newUpper_value;
}

///

void TestCase::injectCaseName()
{
    for(auto&& c : test_case_name_)
        buffer_->appendByte(c);
    buffer_->appendByte('\0');
}

void TestCase::inject()
{
    injectCaseName();

    for(auto&& test : tests_)
    {
        buffer_->appendByte(parser::TestCaseCommand::SENDING_UNIT_TEST_RESULT);
        test->inject();
    }
    buffer_->appendByte(parser::TestCaseCommand::END_SENDING_TEST_CASE);
}

const std::string& TestCase::getTestCaseName() const
{
    return test_case_name_;
}

void TestCase::setTestCaseName(const std::string& newTest_case_name)
{
    test_case_name_ = newTest_case_name;
}

void TestCase::addUnitTest(TestPackPtr test)
{
    tests_.append(test);
}

///

void Transaction::inject()
{
    buffer_->appendByte(parser::GlobalCommand::START);

    for(auto&& test_case : cases_)
    {
        buffer_->appendByte(parser::GlobalCommand::SENDING_TEST_CASE);
        test_case.inject();
    }

    buffer_->appendByte(parser::GlobalCommand::END_ENTIRE_TRANSACTION);
}

void Transaction::addTestCase(TestCase test_case)
{
    cases_.push_back(test_case);
}

void Transaction::setBuffer(port::ByteBuffer* newBuffer)
{
    BufferStorage::setBuffer(newBuffer);

    for(auto&& test_case : cases_)
        test_case.setBuffer(newBuffer);

}

void TestCase::setBuffer(port::ByteBuffer* newBuffer)
{
    BufferStorage::setBuffer(newBuffer);

    for(auto&& test : tests_)
        test->setBuffer(newBuffer);
}
