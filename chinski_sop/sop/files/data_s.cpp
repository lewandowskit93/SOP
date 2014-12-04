#include "./sop/files/data_s.h"
#include "./sop/files/constev.h"
#include "./sop/files/block.h"

#include <iostream>
#include <string>
#include <vector>

/*
32 ch
*/

sop::files::Data_s::Data_s(Data* ptr, sop::logger::Logger* logger) :
  data_p(ptr),
  logger(logger)
{
  this->logger->logFiles(3, "Data serialization member");
}

sop::files::Data_s::~Data_s()
{
}

sop::files::Block* sop::files::Data_s::readData(std::vector<char> dataIn)
{
  this->logger->logFiles(3, "Reading data block");
  std::string paste;
  for(uint32_t iter=0; iter < dataIn.size(); iter++)
  {
    paste += dataIn[iter];
  }
  return new sop::files::Data(paste);
}

std::vector<char> sop::files::Data_s::writeData()
{
  this->logger->logFiles(3, "Writing data block");
  std::vector<char> out;
  std::array<char, sop::files::ConstEV::blockSize> returned = data_p->sop::files::Data::getData_d();
  for(uint32_t iterator=0; iterator < sop::files::ConstEV::blockSize; iterator++)
  {
    out.push_back(returned[iterator]);
  }
  return out;
}