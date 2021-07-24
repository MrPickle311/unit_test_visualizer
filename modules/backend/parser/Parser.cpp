#include "../Parser.hpp"
#include <algorithm>

//Sizes singleton initialization

namespace global
{

template<>
void TypesSizes::initValues()
{
    addValue(TypeDescriptor::BIT, 1);
    addValue(TypeDescriptor::BOOL, 1);
    addValue(TypeDescriptor::CHAR, 1);
    addValue(TypeDescriptor::PTR, 1);
    addValue(TypeDescriptor::UINT8_T, 1);
    addValue(TypeDescriptor::INT8_T, 1);

    addValue(TypeDescriptor::INT16_T, 2);
    addValue(TypeDescriptor::UINT16_T, 2);

    addValue(TypeDescriptor::INT32_T, 4);
    addValue(TypeDescriptor::UINT32_T, 4);

    addValue(TypeDescriptor::INT64_T, 8);
    addValue(TypeDescriptor::UINT64_T, 8);

    is_initialized_ = true;
}

}

namespace backend
{


TypeDescriptor  ParserComponent::current_type_{};

ParserComponent* ParserComponent::getParent()
{
    return  this->parent_;
}

void ParserComponent::setParent(ParserComponent* newParent)
{
    this->parent_ = newParent;
}

void ParserComponent::addChild(uint8_t cmd, QSharedPointer<ParserComponent> child){}

bool ParserComponent::isComposite() const
{
    return false;
}

void ParserComponent::setBuffer(ByteBuffer* newBuffer)
{
    buffer_ = newBuffer;
}

void ParserComponent::createPackage()
{
    package_ = UnitTestPackPtr::create();
}

ComplexParser::ComplexParser(Code commands_count):
    commands_count_{commands_count}{}

bool ComplexParser::isComposite() const
{
    return true;
}

void ComplexParser::addChild(uint8_t cmd, QSharedPointer<ParserComponent> child)
{
    throwIf(child.isNull() , "Child cannot be a nullptr!");

    child->setParent(this);
    child->setBuffer(buffer_);

    children_[cmd] = child;
}

void ComplexParser::checkCode(Code cmd, std::string class_name)
{
    throwIf(cmd >= commands_count_ , { "Command error : " +
                                       class_name +
                                       " , out of range"});
}

void ComplexParser::proccessingLoop()
{
    Code cmd {buffer_->getByte()};
    checkCode(cmd, typeid (*this).name() );

    while (children_[cmd]->parseCommand(package_))//make a map further
    {
        cmd = buffer_->getByte();
        checkCode(cmd, typeid (*this).name() );
    }
}

bool ComplexParser::parseCommand(AcceptedTypes result)
{
    createPackage();

    specialPreOperations(result);

    proccessingLoop();

    specialPostOperations(result);

    return true;
}

void ComplexParser::specialPreOperations([[maybe_unused]] AcceptedTypes result){}

void ComplexParser::specialPostOperations([[maybe_unused]] AcceptedTypes result){}

RootParser::RootParser():
    ComplexParser{GlobalCommand::GLOBAL_COMMAND_COUNT}{}

void RootParser::createPackage()
{
    package_ = TransactionPackPtr::create();
}

TransactionPackPtr RootParser::getPackage()
{
    return std::get<TransactionPackPtr>(package_);
}

void RootParser::startProcessing()
{
    parseCommand(std::monostate{});
}

bool GlobalStartParser::parseCommand(AcceptedTypes result)
{
    return true;
}

bool EndParser::parseCommand(AcceptedTypes result)
{
    return false;
}

TestCaseParser::TestCaseParser():
    ComplexParser{TestCaseCommand::TEST_CASE_COMMAND_COUNT }{}

void TestCaseParser::createPackage()
{
    //create a test case
    package_ = TestCasePackPtr::create();
}

void TestCaseParser::specialPreOperations(AcceptedTypes result)
{
    //append a test case
    std::get<TransactionPackPtr>(result)->addTestCase(std::get<TestCasePackPtr>(package_));

    QByteArray name_bytes;

    char byte {buffer_->getByte()};
    while(byte != '\0')
    {
        name_bytes.append(byte);
        byte = buffer_->getByte();
    }

    std::get<TestCasePackPtr>(package_)->setTestCaseName(name_bytes);
}

UnitTestParser::UnitTestParser():
    ComplexParser{UnitTestCommand::COMMANDS_COUNT}{}

void UnitTestParser::specialPreOperations(AcceptedTypes result)
{
    //append a unit test
    std::get<TestCasePackPtr>(result)->addUnitTest(std::get<UnitTestPackPtr>(package_));
}

bool NameParser::parseCommand(AcceptedTypes result)//unit test
{
    auto test_case {std::get<UnitTestPackPtr>(result)};

    QByteArray name_bytes;

    char byte {buffer_->getByte()};
    while(byte != '\0')
    {
        name_bytes.append(byte);
        byte = buffer_->getByte();
    }

    test_case->setName(name_bytes);

    return true;
}

bool TypeDescriptorParser::parseCommand(AcceptedTypes result)//unit test
{
    auto test_case {std::get<UnitTestPackPtr>(result)};

    QByteArray desc_result;

    desc_result.append(buffer_->getByte());

    current_type_ =  static_cast<TypeDescriptor>((uint8_t)desc_result[0]);

    test_case->setDescriptor(desc_result);

    return true;
}

bool ValueParser::parseCommand(AcceptedTypes result)//unit test
{
    auto test_case {std::get<UnitTestPackPtr>(result)};

    QByteArray value_result;

    for(int i{0}; i < TypesSizes::getValue(current_type_) ; ++i)
        value_result.append(buffer_->getByte());

    redirectValueBytes(value_result, test_case);

    return true;
}

void CurrentValueParser::redirectValueBytes(const QByteArray& value_result, UnitTestPackPtr& unit_test)
{
    unit_test->setCurrentValue(value_result);
}

void ExpectedValueParser::redirectValueBytes(const QByteArray& value_result, UnitTestPackPtr& unit_test)
{
    unit_test->setExpectedValue(value_result);
}

void LowerValueParser::redirectValueBytes(const QByteArray& value_result, UnitTestPackPtr& unit_test)
{
    unit_test->setLowerValue(value_result);
}

void UpperValueParser::redirectValueBytes(const QByteArray& value_result, UnitTestPackPtr& unit_test)
{
    unit_test->setUpperValue(value_result);
}

bool TestResultParser::parseCommand(AcceptedTypes result)//unit test
{
    auto test_case {std::get<UnitTestPackPtr>(result)};

    QByteArray test_result;

    test_result.append(buffer_->getByte());

    test_case->setResult(test_result);

    return true;
}




}



