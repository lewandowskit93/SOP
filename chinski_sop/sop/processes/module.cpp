#include ".\sop\processes\module.h"

sop::processes::Module::Module(sop::system::System *system):
  sop::system::Module(system)
{

}

sop::processes::Module::~Module()
{

}

std::string sop::processes::Module::getClassName() const
{
  return "sop::processes::Module";
}

void sop::processes::Module::initializeModule()
{
}