#include ".\sop\files\file.h"
#include ".\sop\files\filesystem.h"
#include <algorithm>

sop::files::File::File(pid_t* PID, uint32_t parentCatalog, uint32_t blockAddress, std::array<sop::files::Block*, sop::files::ConstEV::numOfBlocks>* disk, sop::logger::Logger* logger) : 
  PIDHolder(PID),
  parentCatalogAddress(parentCatalog),
  blockAddress(blockAddress),
  isDataLoaded(0),
  openMode(0),
  fileName(""),
  UID(0),
  GID(0),
  drive(disk),
  logger(logger)
{
  this->logger->logFiles(6, "File initialization");
  disk->at(blockAddress)->toggleLock();
}

sop::files::File::~File()
{
  if(this->blockAddress > 0 && this->blockAddress < sop::files::ConstEV::blockSize)
  {
    this->drive->at(this->blockAddress)->toggleLock();
  }
}

uint32_t sop::files::File::getBlockAddr()
{
  this->logger->logFiles(3, "File: getting block address = "+std::to_string(this->blockAddress));
  return this->blockAddress;
}

std::vector<std::array<char, sop::files::ConstEV::blockSize>> sop::files::File::getData()
{
  this->logger->logFiles(3, "File: getting data");
  if(!this->isDataLoaded)
  {
    this->loadData();
  }
  return this->data;
}
      
char sop::files::File::getMode()
{
  this->logger->logFiles(3, "File: getting mode");
  return this->openMode;
}

std::string sop::files::File::getFileName()
{
  this->logger->logFiles(3, "File: getting filename");
  return this->fileName;
}

sop::files::uid_t sop::files::File::getUID()
{
  this->logger->logFiles(3, "File: getting UID");
  return this->UID;
}
      
sop::files::gid_t sop::files::File::getGID()
{
  this->logger->logFiles(3, "File: getting GID");
  return this->GID;
}

void sop::files::File::loadData()
{
  this->logger->logFiles(3, "File: loading data");
  sop::files::Block* filenode = this->drive->at(this->blockAddress);
  this->data = filenode->getData_i(this->drive);
  this->isDataLoaded = 1;
}

void sop::files::File::writeToFile(std::string text, std::vector<uint32_t>* freeSpace)
{
  this->logger->logFiles(6, "File: writing to file");
  this->drive->at(this->blockAddress)->writeToFile(text, freeSpace, this->drive);
}

void sop::files::File::removeFile(std::vector<uint32_t>* freeSpace)
{
  this->logger->logFiles(6, "File: removing file");
  this->drive->at(this->blockAddress)->removeFile(freeSpace, this->drive);
  this->drive->at(this->blockAddress) = 0;
  freeSpace->push_back(this->blockAddress);
  std::sort(freeSpace->begin(), freeSpace->end());
}

void sop::files::File::setMode(char mode)
{
  this->logger->logFiles(3, "File: setting mode");
  // TEST if permissions are valid
  this->openMode = mode;
}