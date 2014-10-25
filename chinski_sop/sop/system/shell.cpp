#include ".\sop\system\shell.h"

sop::system::Shell::Shell():
  sop::Object()
{
}

sop::system::Shell::~Shell()
{
}

std::string sop::system::Shell::getClassName() const
{
  return "sop::system::Shell";
}