#include <string>
#include <cstdint>
#include ".\sop\files\block.h"
#include ".\sop\files\data.h"

sop::files::Data::Data()
{
}

sop::files::Data::Data(std::string data)
{
  if(data.size() > sop::files::ConstEV::blockSize) data = data.substr(0, sop::files::ConstEV::blockSize);
  for(uint32_t i=0; i<data.size(); i++)
  {
    this->containter[i] = data[i];
  }
}

sop::files::Data::~Data()
{
}

std::array<char, sop::files::ConstEV::blockSize> sop::files::Data::getData_d()
{
  return this->containter;
}

void sop::files::Data::setData(std::array<char, sop::files::ConstEV::blockSize> data)
{
  this->containter = data;
}