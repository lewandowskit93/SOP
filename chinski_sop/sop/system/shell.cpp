#include <iostream>
#include ".\sop\system\shell.h"
#include ".\sop\system\kernel.h"

sop::system::Shell::Shell(sop::system::Kernel * kernel):
  sop::Object(),
  _kernel(kernel)
{
}

sop::system::Shell::~Shell()
{
}

std::string sop::system::Shell::getClassName() const
{
  return "sop::system::Shell";
}

sop::system::Kernel * sop::system::Shell::getKernel()
{
  return _kernel;
}

const sop::system::Kernel * sop::system::Shell::getKernel() const
{
  return _kernel;
}

void sop::system::Shell::step()
{
  _kernel->getLogger()->logShell(sop::logger::Logger::Level::INFO, "Executing next shell step.");
  if(!std::cin.good())
  {
    _kernel->getLogger()->logShell(sop::logger::Logger::Level::INFO, "End of standard intput. Shutting down the kernel.");
    _kernel->shutdown();
    return;
  }
  std::getline(std::cin,_last_input_line);
  _kernel->getLogger()->logShell(sop::logger::Logger::Level::INFO, "Shell command readed.");
  _kernel->getLogger()->logShell(sop::logger::Logger::Level::FINEST,"Readed line: "+_last_input_line);
}
