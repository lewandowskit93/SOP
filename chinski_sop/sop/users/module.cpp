#include ".\sop\users\module.h"

sop::users::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel)
{

}

sop::users::Module::~Module()
{

}

std::string sop::users::Module::getClassName() const
{
  return "sop::users::Module";
}

void sop::users::Module::initialize()
{
}