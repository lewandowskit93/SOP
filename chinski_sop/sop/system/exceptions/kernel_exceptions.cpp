#include ".\sop\system\exceptions\kernel_exceptions.h"

sop::system::exceptions::KernelException::KernelException():
  sop::Exception()
{
}

sop::system::exceptions::KernelException::KernelException(const std::string & value):
  sop::Exception(value)
{
}

sop::system::exceptions::KernelException::~KernelException()
{
}

std::string sop::system::exceptions::KernelException::getClassName() const
{
  return "sop::system::exceptions::KernelException";
}

sop::system::exceptions::KernelInitializationException::KernelInitializationException():
  sop::system::exceptions::KernelException()
{
}

sop::system::exceptions::KernelInitializationException::KernelInitializationException(const std::string & value):
  sop::system::exceptions::KernelException(value)
{
}

sop::system::exceptions::KernelInitializationException::~KernelInitializationException()
{
}

std::string sop::system::exceptions::KernelInitializationException::getClassName() const
{
  return "sop::system::exceptions::KernelInitializationException";
}

sop::system::exceptions::KernelRunException::KernelRunException():
  sop::system::exceptions::KernelException()
{
}

sop::system::exceptions::KernelRunException::KernelRunException(const std::string & value):
  sop::system::exceptions::KernelException(value)
{
}

sop::system::exceptions::KernelRunException::~KernelRunException()
{
}

std::string sop::system::exceptions::KernelRunException::getClassName() const
{
  return "sop::system::exceptions::KernelRunException";
}

sop::system::exceptions::KernelShutdownException::KernelShutdownException():
  sop::system::exceptions::KernelException()
{
}

sop::system::exceptions::KernelShutdownException::KernelShutdownException(const std::string & value):
  sop::system::exceptions::KernelException(value)
{
}

sop::system::exceptions::KernelShutdownException::~KernelShutdownException()
{
}

std::string sop::system::exceptions::KernelShutdownException::getClassName() const
{
  return "sop::system::exceptions::KernelShutdownException";
}