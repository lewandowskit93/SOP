#include ".\sop\memory\exceptions\module_exception.h"

sop::memory::exceptions::ModuleException::ModuleException():
  sop::system::exceptions::ModuleException()
{
}

sop::memory::exceptions::ModuleException::ModuleException(const std::string & value):
  sop::system::exceptions::ModuleException(value)
{
}

sop::memory::exceptions::ModuleException::~ModuleException()
{
}

std::string sop::memory::exceptions::ModuleException::getClassName() const
{
  return "sop::memory::exceptions::ModuleException";
}