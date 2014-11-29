#include ".\sop\files\inode.h"
#include ".\sop\files\block.h"
#include ".\sop\files\constev.h"
#include ".\sop\files\data.h"

#include <vector>
#include <string>
#include <array>

sop::files::Inode::Inode(bool isDirectory, uid_t UID, gid_t GID) :
  isDirectory(isDirectory),
  uid(UID),
  gid(GID)
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
  if(this->directory.inodesInside.size())
  {
    for(auto x : this->directory.inodesInside)
    {
      output.push_back(x.first);
    }
  }
  else
  {
    output.push_back("0");
  }
  return output;
}

void sop::files::Inode::addInDir(std::string fileName, uint32_t blockAddr)
{
  this->directory.inodesInside[fileName] = blockAddr;
}