#include ".\sop\system\kernel.h"
#include ".\sop\system\module.h"

sop::system::Module::Module(sop::system::Kernel *kernel):
  sop::Object(),
  _kernel(kernel)
{

}

sop::system::Module::~Module()
{

}

std::string sop::system::Module::getClassName() const
{
  return "sop::system::Module";
}

sop::system::Kernel * sop::system::Module::getKernel()
{
  return _kernel;
}

const sop::system::Kernel * sop::system::Module::getKernel() const
{
  return _kernel;
}