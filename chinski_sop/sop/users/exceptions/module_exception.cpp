#include ".\sop\users\exceptions\module_exception.h"

sop::users::exceptions::ModuleException::ModuleException():
  sop::system::exceptions::ModuleException()
{
}

sop::users::exceptions::ModuleException::ModuleException(const std::string & value):
  sop::system::exceptions::ModuleException(value)
{
}

sop::users::exceptions::ModuleException::~ModuleException()
{
}

std::string sop::users::exceptions::ModuleException::getClassName() const
{
  return "sop::users::exceptions::ModuleException";
}