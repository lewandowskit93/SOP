#include <string>
#include <iostream>
#include <cstdint>
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

std::string* sop::files::Filesystem::createFile(pid_t* PID, std::string fileName)
{
  return 0;
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

uint16_t sop::files::Filesystem::renameFile(pid_t* PID, std::string fileName, std::string newFileName)
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
std::string sop::files::Filesystem::getcurrentDir()
{
   return 0;
}

uint16_t sop::files::Filesystem::changeDirectory(pid_t* PID, std::string directoryName)
{
  return 0;
}

uint16_t sop::files::Filesystem::createDirectory(pid_t* PID, std::string newDirectoryName)
{
  return 0;
}

uint16_t sop::files::Filesystem::removeDirectory(pid_t* PID, std::string directoryName)
{
  return 0;
}
// remember to delete all files and return them to freeSpaceVector

uint16_t sop::files::Filesystem::renameDirectory(pid_t* PID, std::string directoryName, std::string newDirectoryName)
{
  return 0;
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
    std::cout<<"cd handler test"<<std::endl;
  }
  else
  {
    std::cout<<"Encountered else"<<std::endl;
  }
}