#include ".\sop\files\inode.h"
#include ".\sop\files\block.h"
#include ".\sop\files\constev.h"
#include ".\sop\files\data.h"

#include <vector>
#include <array>

sop::files::Inode::Inode()
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