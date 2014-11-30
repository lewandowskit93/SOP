#include <string>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <cstdint>
#include <boost\make_shared.hpp>
#include ".\sop\files\filesystem.h"
#include ".\sop\files\file.h"
#include ".\sop\files\inode.h"
#include <iostream>

std::vector<std::string> getPathFromParam(std::string path);

sop::files::Filesystem::Filesystem()
{
  for(uint32_t i=1; i < sop::files::ConstEV::numOfBlocks; i++)
  {
    this->freeSpace.insert(this->freeSpace.end(),i);
    this->dataBlocks[i] = 0;
  }
  this->dataBlocks[0] = new sop::files::Inode(true, 0,0);
  this->printStats();
  this->printDisk(16);
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
  File* out = this->seekForFile(PID, fileName, subdir);
  this->openedFilesList.push_back(out);
  return out;
}

std::string sop::files::Filesystem::readFile(File* fileHandler)
{
  std::string out = "";
  std::vector<std::array<char, sop::files::ConstEV::blockSize>> data = fileHandler->getData();
  for(auto vec : data)
  {
    for(int32_t i=0; i<sop::files::ConstEV::blockSize; i++)
    {
      out+= vec[i];
    }
  }
  return out;
}

/*REWRITE*/
void sop::files::Filesystem::createFile(pid_t* PID, std::string fileName, std::vector<std::string>* subdir)
{
  uint32_t iterator = 0;
  if(this->currentDir.blockRoute.size())
  {
    iterator = this->currentDir.blockRoute.back();
  }
  std::vector<std::string> check = this->dataBlocks[iterator]->listDir();
  for(auto data : check)
  {
    if(data == fileName)
    {
      std::cout<<"File already exist!"<<std::endl;
      return;
    }
  }
  uid_t uid = this->dataBlocks[iterator]->getUID();
  gid_t gid = this->dataBlocks[iterator]->getGID();
  bool writePermission = 1; // sop::user::ask for write permission

  // Rewrite that bit
  if(writePermission)
  {
    uint32_t inodeAddr = this->freeSpace.at(0);
    this->freeSpace.erase(this->freeSpace.begin());
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
    std::cout<<"touch: cannot create '"<<fileName<<"': Permission denied"<<std::endl;
  }
}

void sop::files::Filesystem::saveFile(File* fileHandler)
{

}

void sop::files::Filesystem::closeFile(File* fileHandler)
{
  this->openedFilesList.remove(fileHandler);
  delete fileHandler;
}

void sop::files::Filesystem::removeFile(pid_t* PID, std::string fileName)
{

}

void sop::files::Filesystem::moveFile(pid_t* PID, std::string fileName, std::string newDirectory)
{

}

void sop::files::Filesystem::writeToFile(File* fileHandler, std::string data)
{
  if(!this->dataBlocks[fileHandler->getBlockAddr()]->getIsDirectory())
  {
    this->dataBlocks[fileHandler->getBlockAddr()]->writeToFile(data, &this->freeSpace, &this->dataBlocks);
  }
  else
  {
    std::cout<<fileHandler->getFileName()<<" is a directory!"<<std::endl;
  }
}

sop::files::File* sop::files::Filesystem::seekForFile(pid_t* PID, std::string fileName, std::vector<std::string>* subdir)
{
  return 0; // if file not found or opened
}
    
/* 
  Directories
*/
std::string sop::files::Filesystem::getCurrentDir()
{
  if(this->currentDir.path.size())
  {
    return this->currentDir.path.at(this->currentDir.path.size()-1);
  }
  return "/";
}

std::string sop::files::Filesystem::getCurrentPath()
{
  std::string output = "/";
  if(this->currentDir.path.size())
  {
    for(auto data : this->currentDir.path)
    {
      output += data += "/";
    }
  }
  return output;
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
  this->printStats();
  this->printDisk(16);
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
  return new sop::files::File(-1, -1, -1, &this->dataBlocks); // tmp legacy reasons
}

/*
  Overall
*/
std::vector<std::string> sop::files::Filesystem::list()
{
  uint32_t iterator = 0;
  if(this->currentDir.blockRoute.size())
  {
    iterator = this->currentDir.blockRoute.back();
  }
  return this->dataBlocks[iterator]->listDir();
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
        sop::files::File* directory = this->seekForDirectory(&PID, params.at(1));
        this->currentDir.blockRoute.push_back(directory->getBlockAddr());
        this->currentDir.path.push_back(params.at(1));
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
  std::cout<<"Loading nano..."<<std::endl<<"Nano not yet supported"<<std::endl;
}

void sop::files::Filesystem::createFileHandler(const std::vector<const std::string> & params)
{
  if(params.size() > 1)
  {
    auto path = getPathFromParam(params[1]);
    if(path[0] == " ")
    {
      std::cout<<"Not yet supported"<<std::endl;
    }
    else
    {
      this->createFile(0, path[path.size()-1]);
    }
  }
  this->printStats();
  this->printDisk(16);
}

void sop::files::Filesystem::createDirectoryHandler(const std::vector<const std::string> & params)
{
  if(params.size() > 1)
  {
    auto x = getPathFromParam(params[1]);
  }
}

void sop::files::Filesystem::removeDirectoryHandler(const std::vector<const std::string> & params)
{
  if(params.size() > 1)
  {
    auto x = getPathFromParam(params[1]);
  }
}

void sop::files::Filesystem::catHandler(const std::vector<const std::string> & params)
{
  if(params.size() > 1)
  {
    auto path = getPathFromParam(params[1]);
    if(path[0] == " ")
    {
      std::cout<<"Not yet supported"<<std::endl;
    }
    else
    {
      uint32_t iterator = 0;
      if(this->currentDir.blockRoute.size())
      {
        iterator = this->currentDir.blockRoute.back();
      }
      std::vector<std::string> list = this->dataBlocks[iterator]->listDir();
    }
  }
  }
}

void sop::files::Filesystem::listHandler(const std::vector<const std::string> & params)
{
  std::vector<std::string> x = this->list();
  if(x[0] != "Total: 0")
  {
    std::cout<<"<rwx>\t<user>\t<size>\t<filename>"<<std::endl;
    for(auto data : x)
    {
      std::cout<<"\t\t\t"<<data<<std::endl;
    }
  }
  else
  {
    std::cout<<x[0]<<std::endl;
  }
  this->printStats();
  this->printDisk(16);
}

void sop::files::Filesystem::printStats()
{
  std::cout<<"Free spaces: "<<this->freeSpace.size()<<std::endl;
  std::cout<<"Opened files: "<<this->openedFilesList.size()<<std::endl;
  std::cout<<"Current path: "<<this->getCurrentPath()<<std::endl;
}

void sop::files::Filesystem::printDisk(uint32_t parts)
{
  for(uint32_t iterator=0; iterator < ceil(sop::files::ConstEV::numOfBlocks/parts); iterator++)
  {
    for(uint32_t tmp=0; tmp<(parts*2+1); tmp++)
    {
      std::cout<<"-";
    }
    std::cout<<std::endl<<"|";
    for(uint32_t tmp=0; tmp<parts; tmp++)
    {
      if(this->dataBlocks[iterator*parts + tmp] == 0)
      {
        std::cout<<".|"; // empty
      }
      else if(this->dataBlocks[iterator*parts + tmp]->getGID() == -1)
      {
        std::cout<<"R|"; // raw data
      }
      else if(this->dataBlocks[iterator*parts + tmp]->getIsDirectory())
      {
        std::cout<<"D|"; // directory
      }
      else
      {
        std::cout<<"F|"; // file
      }
    }
    std::cout<<std::endl;
  }
  for(uint32_t tmp=0; tmp<(parts*2+1); tmp++)
  {
    std::cout<<"-";
  }
  std::cout<<std::endl;
}

std::vector<std::string> getPathFromParam(std::string path)
{
  std::string delimiter = "/";
  std::vector<std::string> outPath;
  size_t init = 0;
  if(path.find(delimiter) == std::string::npos)
  {
    outPath.push_back(path);
    return outPath;
  }
  while((init = path.find(delimiter)) != std::string::npos)
  {
    outPath.push_back(path.substr(0,init));
    std::cout<<path.substr(0,init)<<std::endl;
    path.erase(0, init+delimiter.length());
  }
  return outPath;
}