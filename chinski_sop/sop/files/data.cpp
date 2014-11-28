#include <string>
#include <cstdint>
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>
#include ".\sop\files\block.h"
#include ".\sop\files\data.h"
#include ".\sop\files\constev.h"
#include ".\sop\logger\console_logger.h"
#include ".\sop\system\kernel.h"

sop::files::Data::Data()
{
  ConstEV consts;
  this->containter = boost::make_shared<char[]>(consts.blockSize);
}

sop::files::Data::~Data()
{
}

std::string sop::files::Data::getData()
{
  std::string output = this->containter.get();
  return output;
}

void sop::files::Data::setData(std::string data)
{
  ConstEV consts;
  if(data.length() > consts.blockSize)
  {
      throw -1;
  }
  for(int i=0; i< consts.blockSize; i++)
  {
    this->containter[i] = data[i];
  }
}