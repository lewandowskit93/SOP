#include "./sop/files/filesystem_s.h"

#include <iostream>
#include <string>
//#include "./sop/files/block.h"
//#include "./sop/files/data.h"
//#include "./sop/files/inode.h"

/*
freesp=1,2,3,4,5
#$#
<block id=0 type=inode>
dir=1
uid=123
gid=123
string [tab] blockID
<block id=1 type=file size=x-bytes>
x bytes
<block id=2 type=inode>
dir=0
uid=123
gid=123
direct=123,234,345
indirect=
size=123
<block id=3 type=inode/data/empty>
...
*/

sop::files::Filesystem_s::Filesystem_s(Filesystem* fsPtr, sop::logger::Logger* logger) :
  filesystem_p(fsPtr),
  logger(logger)
{
  this->logger->logFiles(3, "Filesystem serialization member");
}

sop::files::Filesystem_s::~Filesystem_s()
{
}

void sop::files::Filesystem_s::readData(std::vector<std::string> input)
{
  this->logger->logFiles(7, "Reading data from disk");
  if(input[0].find("freesp=") != std::string::npos)
  {
    input[0] = input[0].substr(input[0].find("freesp=")+7, input[0].size() - input[0].find("freesp="));
    this->readFreeSpace(input[0]);
  }
  if(input[1].find("#$#") != std::string::npos)
  {
    std::vector<std::string> tmps;
    tmps.push_back(input[2]);
    for(uint32_t i=3; i<input.size(); i++)
    {
      if(input[i].find("<block ") != std::string::npos)
      {
        this->readDataBlocks(tmps);
        tmps.clear();
      }
      tmps.push_back(input[i]);
      std::cout<<input[i]<<std::endl;
    }
    this->readDataBlocks(tmps);
  }
}

void sop::files::Filesystem_s::readFreeSpace(std::string data)
{
  this->filesystem_p->freeSpace.clear();
  data += ",";
  while(data.size())
  {
    this->filesystem_p->freeSpace.push_back(atoi(data.substr(0, data.find(",")).c_str()));
    data.erase(0, data.find(",")+1);
  }
}

void sop::files::Filesystem_s::readDataBlocks(std::vector<std::string> data)
{
  uint32_t addr = atoi(data[0].substr(data[0].find("id=")+3,data[0].find(" ") - data[0].find("id=")).c_str());
  std::string type = data[0].substr(data[0].find("type=")+5, (data[0].find(">") - data[0].find("type="))-1);
  type.erase(type.end()-1);
  if(type == "inode")
  {
    std::vector<std::string> tmp(data);
    tmp.erase(tmp.begin());
    this->filesystem_p->readInode(addr, tmp);
  }
  if(type == "data")
  {
    std::vector<std::string> tmp(data);
    std::string out;
    tmp.erase(tmp.begin());
    if(tmp.size() > 1)
    {
      for(uint32_t i=0; i<tmp.size(); i++)
      {
        out += tmp[i];
        if(i<tmp.size()-1)
        {
          out += "\n";
        }
      }
    }
    else
    {
      out = tmp[0];
    }
    this->filesystem_p->readData(addr, out);
  }
  if(type == "empty")
  {
    if(this->filesystem_p->dataBlocks[addr] != 0)
    {
      delete this->filesystem_p->dataBlocks[addr];
    }
    this->filesystem_p->dataBlocks[addr] = 0;
  }
}

std::vector<std::string> sop::files::Filesystem_s::writeData()
{
  this->logger->logFiles(7, "Writing data to disk");
  std::vector<std::string> output;
  std::string freeSpace = this->writeFreeSpace();
  std::vector<std::string> dataBlocks = this->writeDataBlocks();
  output.push_back(freeSpace);
  output.push_back("#$#");
  for(uint32_t i=0; i<dataBlocks.size(); i++)
  {
    output.push_back(dataBlocks[i]);
  }
  return output;
}

std::string sop::files::Filesystem_s::writeFreeSpace()
{
  this->logger->logFiles(3, "Writing free spaces vector");
  std::string output = "freesp=";
  std::vector<uint32_t> spaces = this->filesystem_p->freeSpace;
  while(spaces.size())
  {
    output+=std::to_string(spaces[0])+",";
    spaces.erase(spaces.begin());
  }
  output.erase(output.end()-1);
  return output;
}

std::vector<std::string> sop::files::Filesystem_s::writeDataBlocks()
{
  this->logger->logFiles(3, "Writing data blocks");
  std::vector<std::string> output;
  //<block id=x type=inode/data/empty>
  for(uint32_t i=0; i<sop::files::ConstEV::numOfBlocks; i++)
  {
    this->logger->logFiles(3, "Writing block "+std::to_string(i));
    if(this->filesystem_p->dataBlocks[i] != 0)
    {
      if(this->filesystem_p->dataBlocks[i]->getGID() == -1)
      {
        output.push_back("<block id="+std::to_string(i)+" type=data>");
        output.push_back(this->filesystem_p->writeData(i));
      }
      else
      {
        output.push_back("<block id="+std::to_string(i)+" type=inode>");
        output.push_back(this->filesystem_p->writeInode(i));
      }
    }
    else
    {
      output.push_back("<block id="+std::to_string(i)+" type=empty>");
    }
  }
  return output;
}