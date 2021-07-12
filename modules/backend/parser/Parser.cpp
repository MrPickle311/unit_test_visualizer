#include "../Parser.hpp"



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


ParserProcessor* AbstractParserProcessor::setNextProcessor(ParserProcessor* next_processor)
{
    this->next_processor_ = next_processor;
    return next_processor;
}

//default behaviour
QByteArray AbstractParserProcessor::process(Code command)
{
    if(this->next_processor_)
        return this->next_processor_->process(command);
    return QByteArray{""};//append empty
}
