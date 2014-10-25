#include ".\sop\memory\module.h"

sop::memory::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel)
{

}

sop::memory::Module::~Module()
{

}

std::string sop::memory::Module::getClassName() const
{
  return "sop::memory::Module";
}

void sop::memory::Module::initialize()
{
}