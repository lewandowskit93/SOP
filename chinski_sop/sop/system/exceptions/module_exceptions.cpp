#include ".\sop\system\exceptions\module_exceptions.h"

sop::system::exceptions::ModuleException::ModuleException():
  sop::Exception()
{
}

sop::system::exceptions::ModuleException::ModuleException(const std::string & value):
  sop::Exception(value)
{
}

sop::system::exceptions::ModuleException::~ModuleException()
{
}

std::string sop::system::exceptions::ModuleException::getClassName() const
{
  return "sop::system::exceptions::ModuleException";
}

sop::system::exceptions::ModuleInitializationException::ModuleInitializationException():
  sop::system::exceptions::ModuleException()
{
}

sop::system::exceptions::ModuleInitializationException::ModuleInitializationException(const std::string & value):
  sop::system::exceptions::ModuleException(value)
{
}

sop::system::exceptions::ModuleInitializationException::~ModuleInitializationException()
{
}

std::string sop::system::exceptions::ModuleInitializationException::getClassName() const
{
  return "sop::system::exceptions::ModuleInitializationException";
}