#include ".\sop\system\system.h"
#include ".\sop\logger\console_logger.h"

sop::system::System::System():
  sop::Object(),
  _logger(new sop::logger::ConsoleLogger)
{
}

sop::system::System::System(uint16_t logging_level):
  sop::Object(),
  _logger(new sop::logger::ConsoleLogger(logging_level))
{
}

sop::system::System::~System()
{
}

std::string sop::system::System::getClassName() const
{
  return "sop::system::System";
}

void sop::system::System::initializeSystem()
{
}

void sop::system::System::runSystem()
{
}

const sop::logger::Logger * sop::system::System::getLogger() const
{
  return _logger.get();
}

sop::logger::Logger * sop::system::System::getLogger()
{
  return _logger.get();
}

const sop::system::Shell * sop::system::System::getShell() const
{
  return &_shell;
}

sop::system::Shell * sop::system::System::getShell()
{
  return &_shell;
}