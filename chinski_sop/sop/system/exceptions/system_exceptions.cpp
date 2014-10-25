#include ".\sop\system\exceptions\system_exceptions.h"

sop::system::exceptions::SystemException::SystemException():
  sop::Exception()
{
}

sop::system::exceptions::SystemException::SystemException(const std::string & value):
  sop::Exception(value)
{
}

sop::system::exceptions::SystemException::~SystemException()
{
}

std::string sop::system::exceptions::SystemException::getClassName() const
{
  return "sop::system::exceptions::SystemException";
}