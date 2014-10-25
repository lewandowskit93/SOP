#include <sstream>
#include ".\sop\exception.h"

sop::Exception::Exception():
  sop::Object(),
  _value(""){
}

sop::Exception::Exception(const std::string & value):
  sop::Object(),
  _value(value){
}

sop::Exception::~Exception(){
}

std::string sop::Exception::getClassName(){
  return "sop::Exception";
}

std::string sop::Exception::getValue(){
  return _value;
}

std::string sop::Exception::toString(){
  std::stringstream stream;
  stream<<this->getClassName()<<":@"<<this<<":"<<this->getValue();
  return stream.str();
}