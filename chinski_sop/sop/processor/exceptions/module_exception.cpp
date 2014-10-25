#include ".\sop\processor\exceptions\module_exception.h"

sop::processor::exceptions::ModuleException::ModuleException():
  sop::system::exceptions::ModuleException()
{
}

sop::processor::exceptions::ModuleException::ModuleException(const std::string & value):
  sop::system::exceptions::ModuleException(value)
{
}

sop::processor::exceptions::ModuleException::~ModuleException()
{
}

std::string sop::processor::exceptions::ModuleException::getClassName() const
{
  return "sop::processor::exceptions::ModuleException";
}