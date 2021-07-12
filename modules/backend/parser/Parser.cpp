#include "../Parser.hpp"



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
