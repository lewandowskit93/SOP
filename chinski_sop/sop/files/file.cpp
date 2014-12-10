#include ".\sop\files\file.h"
#include ".\sop\files\filesystem.h"
#include <algorithm>
#include <boost\shared_ptr.hpp>

sop::files::File::File(boost::shared_ptr<sop::process::Process> PID, uint32_t parentCatalog, uint32_t blockAddress, std::array<sop::files::Block*, sop::files::ConstEV::numOfBlocks>* disk, sop::logger::Logger* logger) : 
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
  if(disk->at(blockAddress)->getLock())
  {
    std::cout<<"File already opened by somebody!"<<std::endl;
    this->logger->logFiles(2, "File already opened");
    drive = nullptr;
  }
  else
  {
    disk->at(blockAddress)->toggleLock();
  }
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
  if(this->logger)
  {
    this->logger->logFiles(3, "File: getting data");
    this->loadData();
    return this->data;
  }
  else
  {
    std::cout<<"Presumeably bad File initialization discovered"<<std::endl;
  }
  return *new std::vector<std::array<char, sop::files::ConstEV::blockSize>>;
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
}

void sop::files::File::writeToFile(std::string text, std::vector<uint32_t>* freeSpace)
{
  if(this->drive !=0 )
  {
    this->logger->logFiles(6, "File: writing to file");
    this->drive->at(this->blockAddress)->writeToFile(text, freeSpace, this->drive);
  }
  else
  {
    std::cout<<"File already open!"<<std::endl;
    this->logger->logFiles(2, "File already opened");
    return;
  }
}

void sop::files::File::removeFile(std::vector<uint32_t>* freeSpace)
{
  if(this->drive != 0)
  {
    this->logger->logFiles(6, "File: removing file");
    this->drive->at(this->blockAddress)->removeFile(freeSpace, this->drive);
    this->drive->at(this->blockAddress) = 0;
    freeSpace->push_back(this->blockAddress);
    std::sort(freeSpace->begin(), freeSpace->end());
  }
  else
  {
    std::cout<<"File already opened"<<std::endl;
    this->logger->logFiles(2, "File already opened");
  }
}

void sop::files::File::setMode(char mode)
{
  this->logger->logFiles(3, "File: setting mode");
  // TEST if permissions are valid
  if(mode == 'w' || mode == 'x')
  {
    sop::users::PermissionsManager pm;
    if(mode == 'w')
    {
     if(pm.hasPermission(this->getInode(), this->PIDHolder, 2))
       this->openMode = mode;
    }
    if(mode == 'x')
    {
      if(pm.hasPermission(this->getInode(), this->PIDHolder, 1))
        this->openMode = mode;
    }
  }
  else
  {
    this->openMode = mode;
  }
}

uint32_t sop::files::File::getSize()
{
  if(this->drive->at(this->blockAddress) != 0)
  {
    return this->drive->at(this->blockAddress)->getSize();
  }
  return 0;
}

void sop::files::File::setFilename(std::string filename)
{
  this->fileName = filename;
}

sop::files::Inode* sop::files::File::getInode()
{
  if(this->drive !=0 && this->blockAddress >= 0 && this->blockAddress < sop::files::ConstEV::numOfBlocks)
  {
    return new Inode(*dynamic_cast<sop::files::Inode*>(this->drive->at(this->blockAddress)));
  }
  return nullptr;
}

boost::shared_ptr<sop::process::Process> sop::files::File::getPID()
{
  return this->PIDHolder;
}

sop::users::Permissions sop::files::File::getPermission()
{
  if(this->drive != 0)
  {
    auto x = this->drive->at(this->blockAddress)->getPermissions();
    return this->drive->at(this->blockAddress)->getPermissions();
  }
  else
  {
    std::cout<<"File already opened"<<std::endl;
    this->logger->logFiles(2, "File already opened");
  }
  return *new sop::users::Permissions();
}