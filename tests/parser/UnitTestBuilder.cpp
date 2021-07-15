#include "UnitTestBuilder.hpp"

void BufferStorage::setBuffer(port::ByteBuffer* newBuffer)
{
    buffer_ = newBuffer;
}

//void UnitTestInjecter::injectName()
//{
//    buffer_->appendByte(parser::UnitTestCommand::SENDING_NAME);
//    for(auto&& c : name_)
//        buffer_->appendByte(c);
//    buffer_->appendByte('\0');
//}
//
//void UnitTestInjecter::injectType()
//{
//    buffer_->appendByte(parser::UnitTestCommand::SENDING_TYPE_DESCRIPTOR);
//    buffer_->appendByte(getDescriptor().data()[0]);
//}
//
//void UnitTestInjecter::injectCurrentValue()
//{
//    buffer_->appendByte(parser::UnitTestCommand::SENDING_CURRENT_VALUE);
//    injectMultiValue(current_value_);
//}
//
//void UnitTestInjecter::injectTestResult()
//{
//    buffer_->appendByte(parser::UnitTestCommand::SENDING_TEST_RESULT);
//    buffer_->appendByte(result_);
//}
//
//void UnitTestInjecter::injectEnd()
//{
//    buffer_->appendByte(parser::UnitTestCommand::END_SENDING_UNIT_TEST_RESULT);
//}
//
//void UnitTestInjecter::injectMultiValue(QList<uint8_t> values)
//{
//    for(auto&& value : values)
//        buffer_->appendByte(value);
//}
//
//void UnitTestInjecter::inject()
//{
//    injectType();
//    injectName();
//    injectCurrentValue();
//    injectValues();
//    injectTestResult();
//    injectEnd();
//}
//
/////
//
//void UnitTestDataPackage::appendExpectedValue()
//{
//    buffer_->appendByte(parser::UnitTestCommand::SENDING_EXPECTED_VALUE);
//    injectMultiValue(current_value_);
//}
//
//void UnitTestDataPackage::injectValues()
//{
//    appendExpectedValue();
//}
//
/////
//
//void RangeUnitTestDataPackage::injectLowerValue()
//{
//    buffer_->appendByte(parser::UnitTestCommand::SENDING_LOWER_VALUE);
//    injectMultiValue(lower_value_);
//}
//
//void RangeUnitTestDataPackage::injectUpperValue()
//{
//    buffer_->appendByte(parser::UnitTestCommand::SENDING_UPPER_VALUE);
//    injectMultiValue(upper_value_);
//}
//
//void RangeUnitTestDataPackage::injectValues()
//{
//    injectLowerValue();
//    injectUpperValue();
//}
//
/////
//
//void TestCase::injectCaseName()
//{
//    for(auto&& c : test_case_name_)
//        buffer_->appendByte(c);
//    buffer_->appendByte('\0');
//}
//
//void TestCase::inject()
//{
//    injectCaseName();
//
//    for(auto&& test : tests_)
//    {
//        buffer_->appendByte(parser::TestCaseCommand::SENDING_UNIT_TEST_RESULT);
//        test->inject();
//    }
//    buffer_->appendByte(parser::TestCaseCommand::END_SENDING_TEST_CASE);
//}
//
//void TestCase::addUnitTest(TestPackPtr test)
//{
//    tests_.append(test);
//}
//
/////
//
//void Transaction::inject()
//{
//    buffer_->appendByte(parser::GlobalCommand::START);
//
//    for(auto&& test_case : cases_)
//    {
//        buffer_->appendByte(parser::GlobalCommand::SENDING_TEST_CASE);
//        test_case->inject();
//    }
//
//    buffer_->appendByte(parser::GlobalCommand::END_ENTIRE_TRANSACTION);
//}
//
//void Transaction::addTestCase(QSharedPointer<TestCase> test_case)
//{
//    cases_.push_back(test_case);
//}
//
//void Transaction::setBuffer(port::ByteBuffer* newBuffer)
//{
//    BufferStorage::setBuffer(newBuffer);
//
//    for(auto&& test_case : cases_)
//        test_case->setBuffer(newBuffer);
//
//}
//
//void TestCase::setBuffer(port::ByteBuffer* newBuffer)
//{
//    BufferStorage::setBuffer(newBuffer);
//
//    for(auto&& test : tests_)
//        test->setBuffer(newBuffer);
//}
//
