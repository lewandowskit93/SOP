#include <string>
#include <iostream>
#include <cstdint>
#include <boost\make_shared.hpp>
#include ".\sop\files\filesystem.h"
#include ".\sop\files\file.h"

sop::files::Filesystem::Filesystem()
{
}

sop::files::Filesystem::~Filesystem()
{
}

// Files
sop::files::File* sop::files::Filesystem::openFile(pid_t* PID, std::string fileName, std::string openMode,std::string subdirectory)
{
  return 0;
}

std::string sop::files::Filesystem::readFile(File* fileHandler)
{
  return 0;
}

void sop::files::Filesystem::createFile(pid_t* PID, std::string fileName)
{
  if(fileName.length() > 3)
  {
    uid_t uid = this->dataBlocks[this->currentDir.blockRoute.back()]->getUID();
    gid_t gid = this->dataBlocks[this->currentDir.blockRoute.back()]->getGID();
    bool writePermission = 1; // sop::user::ask for write permission
    if(writePermission)
    {

    }
    else
    {
      throw -2; //no permission to write
    }
  }
  else
  {
    throw -1; //name too long error
  }
}

uint16_t sop::files::Filesystem::saveFile(File* fileHandler)
{
  return 0;
}

uint16_t sop::files::Filesystem::closeFile(File* fileHandler)
{
  return 0;
}

uint16_t sop::files::Filesystem::removeFile(pid_t* PID, std::string fileName)
{
  return 0;
}

uint16_t sop::files::Filesystem::moveFile(pid_t* PID, std::string fileName, std::string newDirectory)
{
  return 0;
}

uint16_t sop::files::Filesystem::appendToFile(File* fileHandler, std::string data)
{
  return 0;
}

sop::files::File* sop::files::Filesystem::seekForFile(pid_t* PID, std::string fileName)
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
  return new sop::files::File();
}

// Overall
std::vector<std::map<std::string, std::string>> sop::files::Filesystem::list()
{
  return std::vector<std::map<std::string, std::string>> ();
}
// map<drwx - name> 

// Handlers
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