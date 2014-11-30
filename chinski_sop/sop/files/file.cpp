#include ".\sop\files\file.h"
#include ".\sop\files\filesystem.h"

sop::files::File::File(pid_t PID, uint32_t parentCatalog, uint32_t blockAddress, std::array<sop::files::Block*, sop::files::ConstEV::numOfBlocks>* disk) : 
  PIDHolder(PID),
  parentCatalogAddress(parentCatalog),
  blockAddress(blockAddress),
  isDataLoaded(0),
  openMode(0),
  fileName(0),
  UID(0),
  GID(0),
  drive(disk)
{
  disk->at(blockAddress)->toggleLock();
}

sop::files::File::~File()
{
  this->drive->at(this->blockAddress)->toggleLock();
}

uint32_t sop::files::File::getBlockAddr()
{
  return this->blockAddress;
}

std::vector<std::array<char, sop::files::ConstEV::blockSize>> sop::files::File::getData()
{
  if(!this->isDataLoaded)
  {
    this->loadData();
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

void sop::files::File::loadData()
{
  sop::files::Block* filenode = this->drive->at(this->blockAddress);
  this->data = filenode->getData_i(this->drive);
  this->isDataLoaded = 1;
}

void sop::files::File::writeToFile(std::string text, std::vector<uint32_t>* freeSpace)
{
  this->drive->at(this->blockAddress)->writeToFile(text, freeSpace, this->drive);
}

void sop::files::File::removeFile(std::vector<uint32_t>* freeSpace)
{
  this->drive->at(this->blockAddress)->removeFile(freeSpace, this->drive);
  this->drive->at(this->blockAddress) = 0;
  freeSpace->push_back(this->blockAddress);
}

void sop::files::File::setMode(char mode)
{
  // TEST if permissions are valid
  this->openMode = mode;
}