#include ".\sop\files\exceptions\module_exception.h"

sop::files::exceptions::ModuleException::ModuleException():
  sop::system::exceptions::ModuleException()
{
}

sop::files::exceptions::ModuleException::ModuleException(const std::string & value):
  sop::system::exceptions::ModuleException(value)
{
}

sop::files::exceptions::ModuleException::~ModuleException()
{
}

std::string sop::files::exceptions::ModuleException::getClassName() const
{
  return "sop::files::exceptions::ModuleException";
}