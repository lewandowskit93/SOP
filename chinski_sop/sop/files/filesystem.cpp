#include <string>
#include <iostream>
#include <cstdint>
#include <boost\make_shared.hpp>
#include ".\sop\files\filesystem.h"
#include ".\sop\files\file.h"
#include ".\sop\files\inode.h"

sop::files::Filesystem::Filesystem()
{
  for(uint32_t i=1; i < sop::files::ConstEV::numOfBlocks; i++)
  {
    this->freeSpace.insert(this->freeSpace.end(),i);
  }
  std::cout<<this->freeSpace.size()<<std::endl;
  this->dataBlocks[0] = new sop::files::Inode(true, 0,0);
}

sop::files::Filesystem::Filesystem(std::string diskFileName)
{
}

sop::files::Filesystem::~Filesystem()
{
}

// Files
sop::files::File* sop::files::Filesystem::openFile(pid_t* PID, std::string fileName, char openMode, std::vector<std::string>* subdir)
{
  return 0;
}

std::string sop::files::Filesystem::readFile(File* fileHandler)
{
  return 0;
}

void sop::files::Filesystem::createFile(pid_t* PID, std::string fileName, std::vector<std::string>* subdir)
{
  uint32_t iterator = 0;
  if(this->currentDir.blockRoute.size())
  {
    iterator = this->currentDir.blockRoute.back();
  }
  //uid_t uid = this->dataBlocks[iterator]->getUID();
  //gid_t gid = this->dataBlocks[iterator]->getGID();
  bool writePermission = 1; // sop::user::ask for write permission
  if(writePermission)
  {
    std::cout<<this->freeSpace.size()<<std::endl;
    uint32_t inodeAddr = 0;//this->freeSpace;
    //this->freeSpace.erase(this->freeSpace.begin());
    if(subdir)
    {
      Block* root = this->dataBlocks[0];
      for(auto subs : *subdir)
      {
      }
    }
    else
    {
      this->dataBlocks[inodeAddr] = new Inode(false, 0,0);
      if(this->currentDir.blockRoute.size())
      {
        this->dataBlocks[this->currentDir.blockRoute.back()]->addInDir(fileName,inodeAddr);
      }
      else
      {
        this->dataBlocks[0]->addInDir(fileName,inodeAddr);
      }
    }
  }
  else
  {
    throw -1; //no permission to write
  }
}

void sop::files::Filesystem::saveFile(File* fileHandler)
{

}

void sop::files::Filesystem::closeFile(File* fileHandler)
{
  
}

void sop::files::Filesystem::removeFile(pid_t* PID, std::string fileName)
{
}

void sop::files::Filesystem::moveFile(pid_t* PID, std::string fileName, std::string newDirectory)
{

}

void sop::files::Filesystem::appendToFile(File* fileHandler, std::string data)
{

}

sop::files::File* sop::files::Filesystem::seekForFile(pid_t* PID, std::string fileName, std::vector<std::string>* subdir)
{
  return 0;
}
    
// Directories
std::string sop::files::Filesystem::getCurrentDir()
{
   return 0;
}

std::string sop::files::Filesystem::getCurrentPath()
{
  return 0;
}

void sop::files::Filesystem::changeDirectory(pid_t* PID, std::string directoryName)
{
  
}

void sop::files::Filesystem::changeDirectoryUp()
{
  if(this->currentDir.blockRoute.size() > 0)
  {
    this->currentDir.blockRoute.erase(this->currentDir.blockRoute.end());
    this->currentDir.path.erase(this->currentDir.path.end());
  }
}

void sop::files::Filesystem::createDirectory(pid_t* PID, std::string newDirectoryName)
{
  
}

void sop::files::Filesystem::removeDirectory(pid_t* PID, std::string directoryName)
{
  
}
// remember to delete all files and return them to freeSpaceVector

sop::files::File* sop::files::Filesystem::seekForDirectory(pid_t* PID, std::string fileName)
{
  return new sop::files::File(-1, -1, -1);
}

/*
  Overall
*/
std::vector<std::string> sop::files::Filesystem::list()
{
  if(this->currentDir.blockRoute.size())
  {
    return this->dataBlocks[this->currentDir.blockRoute.back()]->listDir();
  }
  else
  {
    std::vector<std::string> out;
    out.push_back("Total: 0");
    return out;
  }
}

/*
  Handlers
*/
void sop::files::Filesystem::changeDirectoryHandler(const std::vector<const std::string> & params)
{
  if(params.size()>1)
  {
    std::cout<<"cd handler test data: "<<params.at(1)<<std::endl;
    if(params.at(1) == "..")
    {
      this->changeDirectoryUp();
    }
    else
    {
      //ask for pid (shell/user)
      sop::files::pid_t PID = 0;
      try
      {
        //sop::files::File* directory = this->seekForDirectory(&PID, params.at(1));
        //this->currentDir.blockRoute.push_back(directory->getBlockAddr());
        //this->currentDir.path.push_back(params.at(1));
      }
      catch(...) // error not found create!!!
      {
        std::cout<<"Directory not found"<<std::endl;
      }
    }
  }
}

void sop::files::Filesystem::moveHandler(const std::vector<const std::string> & params)
{
}

void sop::files::Filesystem::removeFileHandler(const std::vector<const std::string> & params)
{
}

void sop::files::Filesystem::nanoHandler(const std::vector<const std::string> & params)
{
  std::cout<<"Loading nano..."<<std::endl;
}

void sop::files::Filesystem::createFileHandler(const std::vector<const std::string> & params)
{
  if(params.size() > 1)
  {
    for(int i=1; i<params.size(); i++)
    {
      if(params[i].find("/") == std::string::npos)
      {
        this->createFile(0, params[i]);
      }
      else
      {
        std::cout<<"Not supported"<<std::endl;
      }
    }
  }
}

void sop::files::Filesystem::createDirectoryHandler(const std::vector<const std::string> & params)
{
}

void sop::files::Filesystem::removeDirectoryHandler(const std::vector<const std::string> & params)
{
}

void sop::files::Filesystem::catHandler(const std::vector<const std::string> & params)
{
}

void sop::files::Filesystem::listHandler(const std::vector<const std::string> & params)
{
  std::vector<std::string> x = this->list();
  for(auto data : x)
  {
    std::cout<<data<<std::endl;
  }
}