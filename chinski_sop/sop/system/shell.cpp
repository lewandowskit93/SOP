#include <iostream>
#include ".\sop\system\shell.h"
#include ".\sop\system\system.h"

sop::system::Shell::Shell(sop::system::System * system):
  sop::Object(),
  _system(system)
{
}

sop::system::Shell::~Shell()
{
}

std::string sop::system::Shell::getClassName() const
{
  return "sop::system::Shell";
}

sop::system::System * sop::system::Shell::getSystem()
{
  return _system;
}

const sop::system::System * sop::system::Shell::getSystem() const
{
  return _system;
}

void sop::system::Shell::shellStep()
{
  _system->getLogger()->logShell(sop::logger::Logger::Level::INFO, "Executing next shell step.");
  if(!std::cin.good())
  {
    _system->getLogger()->logShell(sop::logger::Logger::Level::INFO, "End of standard intput. Shutting down the system.");
    _system->shutDown();
    return;
  }
  std::getline(std::cin,_last_input_line);
  _system->getLogger()->logShell(sop::logger::Logger::Level::INFO, "Shell command readed.");
  _system->getLogger()->logShell(sop::logger::Logger::Level::FINEST,"Readed line: "+_last_input_line);
}
