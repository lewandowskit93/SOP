#include <sstream>
#include ".\sop\object.h"

sop::Object::Object()
{
}

sop::Object::~Object()
{
}

std::string sop::Object::getClassName() const
{
  return "sop::Object";
}

std::string sop::Object::toString() const
{
  std::stringstream stream;
  stream<<this->getClassName()<<":@"<<this;
  return stream.str();
}