#include ".\sop\processes\exceptions\module_exception.h"

sop::processes::exceptions::ModuleException::ModuleException():
  sop::system::exceptions::ModuleException()
{
}

sop::processes::exceptions::ModuleException::ModuleException(const std::string & value):
  sop::system::exceptions::ModuleException(value)
{
}

sop::processes::exceptions::ModuleException::~ModuleException()
{
}

std::string sop::processes::exceptions::ModuleException::getClassName() const
{
  return "sop::processes::exceptions::ModuleException";
}