#include ".\sop\users\module.h"

sop::users::Module::Module(sop::system::System *system):
  sop::system::Module(system)
{

}

sop::users::Module::~Module()
{

}

std::string sop::users::Module::getClassName() const
{
  return "sop::users::Module";
}

void sop::users::Module::initializeModule()
{
}