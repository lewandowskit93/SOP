#include ".\sop\memory\module.h"

sop::memory::Module::Module(sop::system::System *system):
  sop::system::Module(system)
{

}

sop::memory::Module::~Module()
{

}

std::string sop::memory::Module::getClassName() const
{
  return "sop::memory::Module";
}

void sop::memory::Module::initializeModule()
{
}