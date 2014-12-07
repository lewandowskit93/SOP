#include ".\sop\processor\module.h"
#include ".\sop\system\kernel.h"
#include ".\sop\processor\executive_unit.h"
sop::processor::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel)
{
  this->ExUnit = new ExecutiveUnit(kernel->getLogger());
}

sop::processor::Module::~Module()
{

}

std::string sop::processor::Module::getClassName() const
{
  return "sop::processor::Module";
}

void sop::processor::Module::initialize()
{
  sop::system::Shell* shell = this->_kernel->getShell();
  sop::logger::Logger* logger = this->_kernel->getLogger();
  shell->registerCommand("prstate",&Filesystem::changeDirectoryHandler,(this->fsxxxx));

}