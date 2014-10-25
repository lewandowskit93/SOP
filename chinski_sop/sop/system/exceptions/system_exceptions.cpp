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

sop::system::exceptions::SystemInitializationException::SystemInitializationException():
  sop::system::exceptions::SystemException()
{
}

sop::system::exceptions::SystemInitializationException::SystemInitializationException(const std::string & value):
  sop::system::exceptions::SystemException(value)
{
}

sop::system::exceptions::SystemInitializationException::~SystemInitializationException()
{
}

std::string sop::system::exceptions::SystemInitializationException::getClassName() const
{
  return "sop::system::exceptions::SystemInitializationException";
}

sop::system::exceptions::SystemRunException::SystemRunException():
  sop::system::exceptions::SystemException()
{
}

sop::system::exceptions::SystemRunException::SystemRunException(const std::string & value):
  sop::system::exceptions::SystemException(value)
{
}

sop::system::exceptions::SystemRunException::~SystemRunException()
{
}

std::string sop::system::exceptions::SystemRunException::getClassName() const
{
  return "sop::system::exceptions::SystemRunException";
}

sop::system::exceptions::SystemShutdownException::SystemShutdownException():
  sop::system::exceptions::SystemException()
{
}

sop::system::exceptions::SystemShutdownException::SystemShutdownException(const std::string & value):
  sop::system::exceptions::SystemException(value)
{
}

sop::system::exceptions::SystemShutdownException::~SystemShutdownException()
{
}

std::string sop::system::exceptions::SystemShutdownException::getClassName() const
{
  return "sop::system::exceptions::SystemShutdownException";
}