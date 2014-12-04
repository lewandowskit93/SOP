#include ".\sop\files\inode.h"
#include ".\sop\files\block.h"
#include ".\sop\files\constev.h"
#include ".\sop\files\data.h"

#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <map>

sop::files::Inode::Inode(bool isDirectory, uid_t UID, gid_t GID) :
  isDirectory(isDirectory),
  uid(UID),
  gid(GID),
  lock(0)
{
  if(!isDirectory)
  {
    for(uint32_t i=0; i<sop::files::ConstEV::directAddrBlock; i++)
    {
      this->file.directBlockAddr[i] = 0;
    }
  }
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
  return output;
}

void sop::files::Inode::addInDir(std::string fileName, uint32_t blockAddr)
{
  if(this->isDirectory)
  {
    this->directory.inodesInside[fileName] = blockAddr;
  }
}

void sop::files::Inode::removeFromDir(std::string fileName)
{
  if(this->isDirectory)
  {
    this->directory.inodesInside.erase(fileName);
  }
}

bool sop::files::Inode::getIsDirectory()
{
  return this->isDirectory;
}

void sop::files::Inode::toggleLock()
{
  this->lock = !this->lock;
}

void sop::files::Inode::writeToFile(std::string input, std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive)
{
  std::vector<std::string> insider;
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

void sop::files::Inode::removeFile(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive)
{
  if(!this->isDirectory)
  {
    for(uint32_t iterator=0; iterator < sop::files::ConstEV::directAddrBlock; iterator++)
    {
      if(this->file.directBlockAddr[iterator])
      {
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
      delete drive->at(data);
      freeSpace->push_back(data);
      drive->at(data) = 0;
    }
    this->file.indirectBlockAddr.clear();
  }
}

void sop::files::Inode::removeDir(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive)
{
  if(this->isDirectory)
  {
    uint32_t thisBlockNumber = 0;
    if(this->directory.inodesInside.size() != 0)
    {
      std::string toster;
      std::cout<<"The directory is not empty! Do you fancy removing all files and directories inside? (Y/n)";
      std::cin>>toster;
      if(toster == "Y" || toster == "y")
      {
        for(auto data : this->directory.inodesInside)
        {
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
      }
    }
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
    delete this;
  }
}