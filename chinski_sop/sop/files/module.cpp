#include ".\sop\files\module.h"

sop::files::Module::Module(sop::system::System *system):
  sop::system::Module(system)
{

}

sop::files::Module::~Module()
{

}

std::string sop::files::Module::getClassName() const
{
  return "sop::files::Module";
}

void sop::files::Module::initializeModule()
{
}