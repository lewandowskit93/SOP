#include ".\sop\processes\module.h"

sop::processes::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel)
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