#include ".\sop\files\inode.h"
#include ".\sop\files\block.h"
#include ".\sop\files\constev.h"
#include ".\sop\files\data.h"

#include <vector>
#include <string>
#include <array>
#include <map>

sop::files::Inode::Inode(bool isDirectory, uid_t UID, gid_t GID) :
  isDirectory(isDirectory),
  uid(UID),
  gid(GID),
  lock(0)
{
}

sop::files::Inode::~Inode()
{
}

sop::files::uid_t sop::files::Inode::getUID()
{
  return this->uid;
}

sop::files::gid_t sop::files::Inode::getGID()
{
  return this->gid;
}

std::vector<std::array<char, sop::files::ConstEV::blockSize>> sop::files::Inode::getData_i(std::array<sop::files::Block*, sop::files::ConstEV::numOfBlocks>* disk)
{
  std::vector<std::array<char,sop::files::ConstEV::blockSize>> out;
  for(auto dataInside : this->file.directBlockAddr)
  {
    out.push_back(disk->at(dataInside)->getData_d());
  }
  for(auto dataInside : this->file.indirectBlockAddr)
  {
    out.push_back(disk->at(dataInside)->getData_d());
  }
  return out;
}

std::vector<std::string> sop::files::Inode::listDir()
{
  std::vector<std::string> output;
  if(!this->isDirectory)
  {
    output.push_back("Not a directory");
    return output;
  }
  if(this->directory.inodesInside.size())
  {
    for(auto x : this->directory.inodesInside)
    {
      output.push_back(x.first);
    }
  }
  else
  {
    output.push_back("Total: 0");
  }
  return output;
}

void sop::files::Inode::addInDir(std::string fileName, uint32_t blockAddr)
{
  if(this->isDirectory)
  {
    this->directory.inodesInside[fileName] = blockAddr;
  }
  else
  {
    //throw not a dir error
  }
}

bool sop::files::Inode::getIsDirectory()
{
  return this->isDirectory;
}

void sop::files::Inode::toggleLock()
{
  int32_t tmp = this->lock;
  this->lock = (tmp+1)%2;
}

void sop::files::Inode::writeToFile(std::string input, std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive)
{
  std::vector<std::string> insider;
  while(input.size())
  {
    if(input.size() > sop::files::ConstEV::blockSize)
    {
      insider.push_back(input.substr(0,sop::files::ConstEV::blockSize));
      input = input.substr(sop::files::ConstEV::blockSize, input.size() - sop::files::ConstEV::blockSize);
    }
    else
    {
      if(insider.size() == 0) insider.push_back(input);
      break;
    }
  }
  std::vector<uint32_t> spaces;
  for(uint32_t i=0; i<insider.size(); i++)
  {
    spaces.push_back(freeSpace->at(0));
    freeSpace->erase(freeSpace->begin());
    drive->at(spaces[i]) = new sop::files::Data(insider[i]);
  }
  for(uint32_t i=0; i<spaces.size(); i++)
  {
    if(i < sop::files::ConstEV::directAddrBlock)
    {
      this->file.directBlockAddr[i] = spaces[i];
    }
    else
    {
      this->file.indirectBlockAddr.push_back(spaces[i]);
    }
  }
}

uint32_t sop::files::Inode::getAddress(std::string name)
{
  if(this->directory.inodesInside.find(name) != this->directory.inodesInside.end())
  {
    return this->directory.inodesInside.find(name)->second;
  }
  return 0;
}