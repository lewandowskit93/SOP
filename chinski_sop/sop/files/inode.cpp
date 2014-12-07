#include ".\sop\files\inode.h"
#include ".\sop\files\block.h"
#include ".\sop\files\constev.h"
#include ".\sop\files\data.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <array>
#include <map>

bool compareDirList(sop::files::dirList i, sop::files::dirList j)
{
  return (i.name<j.name);
}

sop::files::Inode::Inode(bool isDirectory, uid_t UID, gid_t GID, sop::logger::Logger* logger) :
  isDirectory(isDirectory),
  uid(UID),
  gid(GID),
  lock(0),
  logger(logger)
{
  for(uint32_t i=0; i<sop::files::ConstEV::directAddrBlock; i++)
  {
    this->file.directBlockAddr[i] = 0;
  }
  file.size=0;
}

sop::files::Inode::~Inode()
{
  this->logger->logFiles(3, "Destroing Inode");
}

sop::files::uid_t sop::files::Inode::getUID()
{
  //this->logger->logFiles(3, "Inode getting UID");
  return this->uid;
}

sop::files::gid_t sop::files::Inode::getGID()
{
  //this->logger->logFiles(3, "Inode getting GID");
  return this->gid;
}

std::vector<std::array<char, sop::files::ConstEV::blockSize>> sop::files::Inode::getData_i(std::array<sop::files::Block*, sop::files::ConstEV::numOfBlocks>* disk)
{
  this->logger->logFiles(3, "Inode getting data");
  std::vector<std::array<char,sop::files::ConstEV::blockSize>> out;
  if(this->file.size)
  {
    for(auto dataInside : this->file.directBlockAddr)
    {
      if(dataInside < sop::files::ConstEV::numOfBlocks)
      {
        out.push_back(disk->at(dataInside)->getData_d());
      }
    }
    for(auto dataInside : this->file.indirectBlockAddr)
    {
      if(dataInside < sop::files::ConstEV::numOfBlocks)
      {
        out.push_back(disk->at(dataInside)->getData_d());
      }
    }
  }
  return out;
}

std::vector<sop::files::dirList> sop::files::Inode::listDir(std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk)
{
  this->logger->logFiles(3, "Inode listing initilized");
  std::vector<sop::files::dirList> dirs;
  std::vector<sop::files::dirList> files;
  if(!this->isDirectory)
  {
    this->logger->logFiles(2, "Inode: Not a directory");
    return dirs;
  }
  if(this->directory.inodesInside.size())
  {
    for(auto x : this->directory.inodesInside)
    {
      sop::files::dirList out;
      if(disk->at(x.second) != 0)
      {
        if(disk->at(x.second)->getIsDirectory())
        {
          this->logger->logFiles(3, "Found a directory");
          out.drwx = "d";
          out.size = "-";
        }
        else
        {
          this->logger->logFiles(3, "Found a file");
          out.drwx = "-";
          out.size = std::to_string(disk->at(x.second)->getSize());
        }
        out.drwx += "r--r--r--"; //TEST
        out.name = x.first;
      }
      else
      {
        this->logger->logFiles(1, "Data is corrupted at "+std::to_string(x.second)+" block. Directory member not found.");
      }
      if(disk->at(x.second)->getIsDirectory())
      {
        dirs.push_back(out);
      }
      else
      {
        files.push_back(out);
      }
    }
    this->logger->logFiles(3, "Inode sorting lists");
    std::sort(dirs.begin(), dirs.end(), compareDirList);
    std::sort(files.begin(), files.end(), compareDirList);
    if(files.size())
    {
      for(uint32_t i=0; i<files.size(); i++)
      {
        dirs.push_back(files[i]);
      }
    }
  }
  this->logger->logFiles(6, "Inode listing successful");
  return dirs;
}

void sop::files::Inode::addInDir(std::string fileName, uint32_t blockAddr)
{
  this->logger->logFiles(3, "Inode: adding directory member");
  if(this->isDirectory)
  {
    this->directory.inodesInside[fileName] = blockAddr;
  }
}

void sop::files::Inode::removeFromDir(std::string fileName)
{
  this->logger->logFiles(3, "Inode: removing directory member");
  if(this->isDirectory)
  {
    this->directory.inodesInside.erase(fileName);
  }
}

bool sop::files::Inode::getIsDirectory()
{
  //this->logger->logFiles(3, "Inode: getting if this is a directory");
  return this->isDirectory;
}

void sop::files::Inode::toggleLock()
{
  if(this->lock)
  {
    this->logger->logFiles(6, "Inode: unlocked");
  }
  else
  {
    this->logger->logFiles(6, "Inode: locked");
  }
  this->lock = !this->lock;
}

void sop::files::Inode::writeToFile(std::string input, std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive)
{
  if(this->isDirectory)
  {
    std::cout<<"This is a directory!"<<std::endl;
    this->logger->logFiles(2, "This is a directory");
    return;
  }
  this->logger->logFiles(6, "Inode: writing to file initilized");
  std::vector<std::string> insider;
  uint32_t size=input.size();
  this->logger->logFiles(3, "Inode: input check");
  while(input.size())
  {
    if(input.size() > sop::files::ConstEV::blockSize)
    {
      insider.push_back(input.substr(0,sop::files::ConstEV::blockSize));
      input = input.substr(sop::files::ConstEV::blockSize, input.size()-1);
    }
    else
    {
      insider.push_back(input);
      input.clear();
    }
  }

  // Clear memory if needed
  if(this->file.indirectBlockAddr.size() > 0)
  {
    for(auto blocks : this->file.indirectBlockAddr)
    {
      freeSpace->push_back(blocks);
      std::sort(freeSpace->begin(), freeSpace->end());
      delete drive->at(blocks);
      drive->at(blocks) = 0;
    }
  }
  for(uint32_t i=0; i<sop::files::ConstEV::directAddrBlock; i++)
  {
    if(this->file.directBlockAddr[i] != 0)
    {
      if(i == 0)
      {
        this->logger->logFiles(3, "Inode: file cleared");
      }
      freeSpace->push_back(this->file.directBlockAddr[i]);
      std::sort(freeSpace->begin(), freeSpace->end());
      delete drive->at(this->file.directBlockAddr[i]);
      drive->at(this->file.directBlockAddr[i]) = 0;
    }
  }

  // Reserve memory
  std::vector<uint32_t> spaces;
  this->logger->logFiles(3, "Inode: reserve memory");
  if(insider.size() > freeSpace->size())
  {
    std::cout<<"Insufficeint memory"<<std::endl;
    this->logger->logFiles(2, "Insufficient memory");
    return;
  }
  for(uint32_t i=0; i<insider.size(); i++)
  {
    spaces.push_back(freeSpace->at(0));
    freeSpace->erase(freeSpace->begin());
    drive->at(spaces[i]) = new sop::files::Data(insider[i]);
  }
  this->file.size = size;

  // Alocate memory
  this->logger->logFiles(3, "Inode: alocate memory");
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
  this->logger->logFiles(6, "Inode: writing to file successful");
}

uint32_t sop::files::Inode::getAddress(std::string name)
{
  if(this->directory.inodesInside.find(name) != this->directory.inodesInside.end())
  {
    return this->directory.inodesInside.find(name)->second;
  }
  return 0;
}

void sop::files::Inode::removeFile(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive)
{
  this->logger->logFiles(6, "Inode: removing file initilized");
  if(!this->isDirectory)
  {
    for(uint32_t iterator=0; iterator < sop::files::ConstEV::directAddrBlock; iterator++)
    {
      if(this->file.directBlockAddr[iterator])
      {
        this->logger->logFiles(3, "Inode: block "+std::to_string(iterator)+" has been emptied");
        delete drive->at(this->file.directBlockAddr[iterator]);
        freeSpace->push_back(this->file.directBlockAddr[iterator]);
        this->file.directBlockAddr[iterator] = 0;
        drive->at(iterator) = 0;
      }
      else
      {
        return;
      }
    }
    for(auto data : this->file.indirectBlockAddr)
    {
      this->logger->logFiles(3, "Inode: block "+std::to_string(data)+" has been emptied");
      delete drive->at(data);
      freeSpace->push_back(data);
      drive->at(data) = 0;
    }
    this->file.indirectBlockAddr.clear();
  }
  this->logger->logFiles(6, "Inode: removing file successful");
}

// Throws error if not empty
void sop::files::Inode::removeDir(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive)
{
  this->logger->logFiles(6, "Inode: removing directory initialized");
  if(this->isDirectory)
  {
    uint32_t thisBlockNumber = 0;
    if(this->directory.inodesInside.size() != 0)
    {
      this->logger->logFiles(2, "Inode: directory not empty");
      std::string toster;
      std::cout<<"The directory is not empty!"<<std::endl;
      throw std::string("error");
      /*std::cin>>toster;
      if(toster == "Y" || toster == "y")
      {
        for(auto data : this->directory.inodesInside)
        {
          this->logger->logFiles(6, "Inode: removing "+data.first);
          std::cout<<"Removing: "<<data.first;
          if(drive->at(data.second)->getIsDirectory())
          {
            drive->at(data.second)->removeDir(freeSpace, drive);
          }
          else
          {
            drive->at(data.second)->removeFile(freeSpace, drive);
          }
        }
      }
      else
      {
        std::cout<<"The files are staying as well as a directory. Think what you gonna delete before hitting enter!"<<std::endl;
      }*/
    }
    this->logger->logFiles(3, "Inode: scanning blocks");
    for(uint32_t i=0; i<sop::files::ConstEV::numOfBlocks; i++)
    {
      if(drive->at(i) == this)
      {
        thisBlockNumber = i;
        break;
      }
    }
    freeSpace->push_back(thisBlockNumber);
    drive->at(thisBlockNumber) = 0;
    this->logger->logFiles(6, "Inode: removing successful");
    delete this;
  }
}

uint32_t sop::files::Inode::getSize()
{
  this->logger->logFiles(3, "Inode: getting file size");
  if(!this->isDirectory)
  {
    return this->file.size;
  }
  else
  {
    return 0;
  }
}