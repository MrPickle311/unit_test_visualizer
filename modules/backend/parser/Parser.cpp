#include "../Parser.hpp"

namespace parser
{

void LocalByteParser::parseCommand(Command* cmd)
{

}

bool LocalByteParser::packageReady()
{

}

DataPackage LocalByteParser::getParsedPackage()
{

}

bool LocalByteParser::atStart()
{

}

void LocalByteParser::setBuffer(port::ByteBuffer* buffer)
{
    this->buffer_ = buffer;
}

port::ByteBuffer* LocalByteParser::getBuffer()
{
    return buffer_;
}


void UnitTestLocalByteParser::parseCommand(Command* cmd)
{

}

bool UnitTestLocalByteParser::packageReady()
{
}

DataPackage UnitTestLocalByteParser::getParsedPackage()
{
}

bool UnitTestLocalByteParser::atStart()
{
}

void UnitTestLocalByteParser::setBuffer(port::ByteBuffer* buffer)
{

}

port::ByteBuffer* UnitTestLocalByteParser::getBuffer()
{
}

Processor::Processor(port::ByteBuffer* buffer_):
    buffer_{buffer_}
{}

void Processor::setBuffer(port::ByteBuffer* newBuffer)
{
    buffer_ = newBuffer;
}

QByteArray parser::NameProcessor::process()
{
    QByteArray result;

    char byte {buffer_->getByte()};
    while(byte != '\0')
    {
        result.append(byte);
        byte = buffer_->getByte();
    }

    return result;
}

}

QByteArray parser::TypeDescriptorProcessor::process()
{
    QByteArray result;

    result.append(buffer_->getByte());

    return result;
}
