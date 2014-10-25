#include ".\sop\files\module.h"

sop::files::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel)
{

}

sop::files::Module::~Module()
{

}

std::string sop::files::Module::getClassName() const
{
  return "sop::files::Module";
}

void sop::files::Module::initialize()
{
}