#include <iostream>
#include ".\sop\logger\console_logger.h"

sop::logger::ConsoleLogger::ConsoleLogger():
  Logger()
{
}

sop::logger::ConsoleLogger::ConsoleLogger(uint16_t logging_level):
  Logger(logging_level)
{
}

sop::logger::ConsoleLogger::~ConsoleLogger()
{
}

std::string sop::logger::ConsoleLogger::getClassName()
{
  return "sop::logger::ConsoleLogger";
}

void sop::logger::ConsoleLogger::saveLog(uint16_t channel, uint16_t level, const std::string & message)
{
  std::cout<<Channel::getChannelName(channel)<<":"<<Level::getLevelName(level)<<":"<<message<<std::endl;
}