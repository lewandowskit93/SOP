#include ".\sop\files\file.h"
#include ".\sop\files\filesystem.h"

sop::files::File::File(pid_t PID, uint32_t parentCatalog, uint32_t blockAddress) : 
  PIDHolder(PID),
  parentCatalogAddress(parentCatalog),
  blockAddress(blockAddress),
  isDataLoaded(0),
  openMode(0),
  fileName(0),
  UID(0),
  GID(0)
{
}

sop::files::File::~File()
{
}

uint32_t sop::files::File::getBlockAddr()
{
  return this->blockAddress;
}

std::vector<std::array<char, sop::files::ConstEV::blockSize>> sop::files::File::getData(std::array<sop::files::Block*, sop::files::ConstEV::numOfBlocks>* drive)
{
  if(!this->isDataLoaded)
  {
    this->loadData(drive);
  }
  return this->data;
}
      
char sop::files::File::getMode()
{
  return this->openMode;
}

std::string sop::files::File::getFileName()
{
  return this->fileName;
}

sop::files::uid_t sop::files::File::getUID()
{
  return this->UID;
}
      
sop::files::gid_t sop::files::File::getGID()
{
  return this->GID;
}

void sop::files::File::loadData(std::array<sop::files::Block*, sop::files::ConstEV::numOfBlocks>* drive)
{
  sop::files::Block* filenode = drive->at(this->blockAddress);
  this->data = filenode->getData_i(drive);
  this->isDataLoaded = 1;
}