#include "../Parser.hpp"
#include <algorithm>

namespace parser
{

//Sizes singleton initialization

QMutex TypesSizes::mutex_{};
QMap<TypeDescriptor , int> TypesSizes::sizes_{};
bool TypesSizes::sizes_initialized_{false};

void TypesSizes::addSize(TypeDescriptor desc , int size)
{
    sizes_[desc] = size;
}

void TypesSizes::initSizes()
{
    addSize(TypeDescriptor::BIT, 1);
    addSize(TypeDescriptor::BOOL, 1);
    addSize(TypeDescriptor::CHAR, 1);
    addSize(TypeDescriptor::PTR, 1);
    addSize(TypeDescriptor::UINT8_T, 1);
    addSize(TypeDescriptor::INT8_T, 1);

    addSize(TypeDescriptor::INT16_T, 2);
    addSize(TypeDescriptor::UINT16_T, 2);

    addSize(TypeDescriptor::INT32_T, 4);
    addSize(TypeDescriptor::UINT32_T, 4);

    addSize(TypeDescriptor::INT64_T, 8);
    addSize(TypeDescriptor::UINT64_T, 8);

    sizes_initialized_ = true;
}

int TypesSizes::getSize(TypeDescriptor desc)
{
    QMutexLocker{&mutex_};
    if(not sizes_initialized_)
        initSizes();
    return sizes_[desc];
}

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

void ParserComponent::setBuffer(port::ByteBuffer* newBuffer)
{
    buffer_ = newBuffer;
}

void ParserComponent::createPackage()
{
    package_ = QSharedPointer<UnitTestDataPackage>::create();
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

void ComplexParser::specialPreOperations(AcceptedTypes result){}

void ComplexParser::specialPostOperations(AcceptedTypes result){}

GlobalParser::GlobalParser():
    ComplexParser{GlobalCommand::GLOBAL_COMMAND_COUNT}{}

void GlobalParser::createPackage()
{
    package_ = QSharedPointer<TransactionDataPackage>::create();
}

QSharedPointer<TransactionDataPackage> GlobalParser::getPackage()
{
    return std::get<QSharedPointer<TransactionDataPackage>>(package_);
}

void GlobalParser::startProcessing()
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
    package_ = QSharedPointer<TestCaseDataPackage>::create();
}

void TestCaseParser::specialPreOperations(AcceptedTypes result)
{
    //append a test case
    std::get<QSharedPointer<TransactionDataPackage>>(result)->addTestCase(std::get<QSharedPointer<TestCaseDataPackage>>(package_));

    QByteArray name_bytes;

    char byte {buffer_->getByte()};
    while(byte != '\0')
    {
        name_bytes.append(byte);
        byte = buffer_->getByte();
    }

    std::get<QSharedPointer<TestCaseDataPackage>>(package_)->setTestCaseName(name_bytes);
}

UnitTestParser::UnitTestParser():
    ComplexParser{UnitTestCommand::COMMANDS_COUNT}{}

void UnitTestParser::specialPreOperations(AcceptedTypes result)
{
    //append a unit test
    std::get<QSharedPointer<TestCaseDataPackage>>(result)->addUnitTest(std::get<QSharedPointer<UnitTestDataPackage>>(package_));
}

bool NameParser::parseCommand(AcceptedTypes result)//unit test
{
    auto test_case {std::get<QSharedPointer<UnitTestDataPackage>>(result)};

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
    auto test_case {std::get<QSharedPointer<UnitTestDataPackage>>(result)};

    QByteArray desc_result;

    desc_result.append(buffer_->getByte());

    current_type_ =  static_cast<TypeDescriptor>((uint8_t)desc_result[0]);

    test_case->setDescriptor(desc_result);

    return true;
}

bool ValueParser::parseCommand(AcceptedTypes result)//unit test
{
    auto test_case {std::get<QSharedPointer<UnitTestDataPackage>>(result)};

    QByteArray value_result;

    for(int i{0}; i < TypesSizes::getSize(current_type_) ; ++i)
        value_result.append(buffer_->getByte());

    redirectValueBytes(value_result, test_case);

    return true;
}

void CurrentValueParser::redirectValueBytes(const QByteArray& value_result, QSharedPointer<UnitTestDataPackage>& unit_test)
{
    unit_test->setCurrentValue(value_result);
}

void ExpectedValueParser::redirectValueBytes(const QByteArray& value_result, QSharedPointer<UnitTestDataPackage>& unit_test)
{
    unit_test->setExpectedValue(value_result);
}

void LowerValueParser::redirectValueBytes(const QByteArray& value_result, QSharedPointer<UnitTestDataPackage>& unit_test)
{
    unit_test->setLowerValue(value_result);
}

void UpperValueParser::redirectValueBytes(const QByteArray& value_result, QSharedPointer<UnitTestDataPackage>& unit_test)
{
    unit_test->setUpperValue(value_result);
}

bool TestResultParser::parseCommand(AcceptedTypes result)//unit test
{
    auto test_case {std::get<QSharedPointer<UnitTestDataPackage>>(result)};

    QByteArray test_result;

    test_result.append(buffer_->getByte());

    test_case->setResult(test_result);

    return true;
}




}



