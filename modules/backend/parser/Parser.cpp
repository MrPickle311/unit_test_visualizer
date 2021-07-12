#include "../Parser.hpp"
#include <algorithm>

namespace parser
{

//void LocalByteParser::parseCommand(Command* cmd)
//{
//
//}
//
//bool LocalByteParser::packageReady()
//{
//
//}
//
//DataPackage LocalByteParser::getParsedPackage()
//{
//
//}
//
//bool LocalByteParser::atStart()
//{
//
//}
//
//void LocalByteParser::setBuffer(port::ByteBuffer* buffer)
//{
//    this->buffer_ = buffer;
//}
//
//port::ByteBuffer* LocalByteParser::getBuffer()
//{
//    return buffer_;
//}

UnitTestLocalByteParser::UnitTestLocalByteParser(port::ByteBuffer* buffer) :
    buffer_(buffer),
    result_{},
    package_ready_{false}
{
    for(int i{0} ; i < UnitTestCommand::COMMANDS_COUNT - 1 ; ++i)// COMMANDS_COUNT - 1 -> aborting last command
        result_.parsed_data_.push_back({});
}

void UnitTestLocalByteParser::parseCommand(Command* cmd)
{

}

bool UnitTestLocalByteParser::packageReady() const
{
    return package_ready_;
}

DataPackage UnitTestLocalByteParser::getParsedPackage()
{
    package_ready_ = false;
    return  std::move(this->result_);
}

bool UnitTestLocalByteParser::isEmptyResult() const
{
    return result_.parsed_data_.isEmpty();
}

void UnitTestLocalByteParser::setBuffer(port::ByteBuffer* buffer)
{

}

port::ByteBuffer* UnitTestLocalByteParser::getBuffer()
{
}

void UnitTestLocalByteParser::parseData()
{
    Code cmd {buffer_->getByte()};

    while (cmd != UnitTestCommand::END_SENDING_UNIT_TEST_RESULT)
    {
        checkCode(cmd);

        Processor* proc {Processors::getProcessor(static_cast<UnitTestCommand>(cmd))};

        QByteArray bytes {proc->process(buffer_)};

        result_.parsed_data_.replace(cmd, bytes);
        cmd = buffer_->getByte();
    }

    this->package_ready_ = true;

}

void UnitTestLocalByteParser::checkCode(Code cmd)
{
    throwIf(cmd >= 8 , "Command error : bad command , out of range");
}


QByteArray parser::NameProcessor::process(port::ByteBuffer* buffer)
{
    QByteArray result;

    char byte {buffer->getByte()};
    while(byte != '\0')
    {
        result.append(byte);
        byte = buffer->getByte();
    }

    return result;
}

QByteArray ValueProcessor::process(port::ByteBuffer* buffer)
{
}

QByteArray parser::TypeDescriptorProcessor::process(port::ByteBuffer* buffer)
{
    QByteArray result;

    result.append(buffer->getByte());

    return result;
}


QByteArray CurrentValueProcessor::process(port::ByteBuffer* buffer)
{
    QByteArray result;

    result.append(buffer->getByte());

    return result;
}

QByteArray ExpectedValueProcessor::process(port::ByteBuffer* buffer)
{
    QByteArray result;

    result.append(buffer->getByte());

    return result;
}

QByteArray TestResultProcessor::process(port::ByteBuffer* buffer)
{
    QByteArray result;

    result.append(buffer->getByte());

    return result;
}


void Processors::setCurrentType(TypeDescriptor newCurrent_type)
{
    current_type_ = newCurrent_type;
}

void Processors::initProcessors()
{
    addProcessor<NameProcessor>(UnitTestCommand::SENDING_NAME);
    addProcessor<TypeDescriptorProcessor>(UnitTestCommand::SENDING_TYPE_DESCRIPTOR);
    addProcessor<CurrentValueProcessor>(UnitTestCommand::SENDING_CURRENT_VALUE);
    addProcessor<ExpectedValueProcessor>(UnitTestCommand::SENDING_EXPECTED_VALUE);
    addProcessor<TestResultProcessor>(UnitTestCommand::SENDING_TEST_RESULT);

    processors_initialized_ = true;
}

Processor* Processors::getProcessor(UnitTestCommand cmd)
{
    QMutexLocker{&mutex_};
    if(not processors_initialized_)
        initProcessors();
    return processors_[cmd].get();
}

//Processor singleton initialization
QMutex Processors::mutex_{};
QMap<UnitTestCommand,std::shared_ptr<Processor>> Processors::processors_;
bool Processors::processors_initialized_{false};
TypeDescriptor Processors::current_type_{};

template<typename T>
void Processors::addProcessor(UnitTestCommand code)
{
    processors_[code] = std::make_shared<T>();
}

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

}
