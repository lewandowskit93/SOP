#include ".\sop\processor\module.h"

sop::processor::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel)
{

}

sop::processor::Module::~Module()
{

}

std::string sop::processor::Module::getClassName() const
{
  return "sop::processor::Module";
}

void sop::processor::Module::initializeModule()
{
}