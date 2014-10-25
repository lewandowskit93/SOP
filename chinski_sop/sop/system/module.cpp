#include ".\sop\system\system.h"
#include ".\sop\system\module.h"

sop::system::Module::Module(sop::system::System *system):
  sop::Object(),
  _system(system)
{

}

sop::system::Module::~Module()
{

}

std::string sop::system::Module::getClassName() const
{
  return "sop::system::Module";
}