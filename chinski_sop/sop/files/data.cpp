#include <string>
#include <cstdint>
#include ".\sop\files\block.h"
#include ".\sop\files\data.h"

sop::files::Data::Data()
{
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