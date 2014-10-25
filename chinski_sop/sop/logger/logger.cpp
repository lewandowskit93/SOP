#include <sstream>
#include ".\sop\logger\logger.h"

sop::logger::Logger::Channel::Channel()
{
}

std::string sop::logger::Logger::Channel::getChannelName(uint16_t channel)
{
  switch(channel)
  {
    case USERS_CHANNEL:
      return "USERS";
    case FILES_CHANNEL:
      return "FILES";
    case PROCESSES_CHANNEL:
      return "PROCESSES";
    case MEMORY_CHANNEL:
      return "MEMORY";
    case PROCESSOR_CHANNEL:
      return "PROCESSOR";
    case KERNEL_CHANNEL:
      return "KERNEL_CHANNEL";
    case SHELL_CHANNEL:
      return "SHELL_CHANNEL";
    default:
    {
      std::stringstream stream;
      stream<<channel;
      return stream.str();
    }
  }
}

sop::logger::Logger::Level::Level()
{
}

std::string sop::logger::Logger::Level::getLevelName(uint16_t level)
{
  switch(level)
  {
    case SEVERE:
      return "SEVERE";
    case WARNING:
      return "WARNING";
    case INFO:
      return "INFO";
    case CONFIG:
      return "CONFIG";
    case FINE:
      return "FINE";
    case FINER:
      return "FINER";
    case FINEST:
      return "FINEST";
    default:
    {
      std::stringstream stream;
      stream<<level;
      return stream.str();
    }
  }
}

sop::logger::Logger::Logger():
  sop::Object(),
  _logging_level(Level::SEVERE)
{
}

sop::logger::Logger::Logger(uint16_t logging_level):
  sop::Object(),
  _logging_level(logging_level)
{
}

sop::logger::Logger::~Logger()
{
}

std::string sop::logger::Logger::getClassName() const
{
  return "sop::logger::Logger";
}

void sop::logger::Logger::log(uint16_t channel, uint16_t level, const std::string & message)
{
  if(canLog(level))
  {
    saveLog(channel,level,message);
  }
}

void sop::logger::Logger::logUsers(uint16_t level, const std::string & message)
{
  log(Channel::USERS_CHANNEL,level,message);
}

void sop::logger::Logger::logFiles(uint16_t level, const std::string & message)
{
  log(Channel::FILES_CHANNEL,level,message);
}

void sop::logger::Logger::logProcesses(uint16_t level, const std::string & message)
{
  log(Channel::PROCESSES_CHANNEL,level,message);
}

void sop::logger::Logger::logMemory(uint16_t level, const std::string & message)
{
  log(Channel::MEMORY_CHANNEL,level,message);
}

void sop::logger::Logger::logProcessor(uint16_t level, const std::string & message)
{
  log(Channel::PROCESSOR_CHANNEL,level,message);
}

void sop::logger::Logger::logKernel(uint16_t level, const std::string & message)
{
  log(Channel::KERNEL_CHANNEL,level,message);
}

void sop::logger::Logger::logShell(uint16_t level, const std::string & message)
{
  log(Channel::SHELL_CHANNEL,level,message);
}

uint16_t sop::logger::Logger::getLoggingLevel() const
{
  return _logging_level;
}

void sop::logger::Logger::setLoggingLevel(uint16_t level)
{
  this->_logging_level=level;
}

bool sop::logger::Logger::canLog(uint16_t level)
{
  if(level == Level::OFF) return false;
  if(_logging_level == Level::OFF) return false;
  return level <= _logging_level;
}