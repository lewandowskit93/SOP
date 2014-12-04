#include <string>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <cstdint>
#include <boost\make_shared.hpp>
#include ".\sop\files\filesystem.h"
#include ".\sop\files\file.h"
#include ".\sop\files\inode.h"
#include ".\sop\logger\logger.h"
#include <iostream>

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
  if(path.size() == 1 && path == "/")
  {
    outPath.push_back("");
    return outPath;
  }
  init = path.find(delimiter);
  do
  {
    if(init)
    {
      outPath.push_back(path.substr(0,init));
      std::cout<<path.substr(0,init)<<std::endl;
      path.erase(0, init+delimiter.length());
    }
    else
    {
      outPath.push_back("/");
      std::cout<<"/"<<std::endl;
      path.erase(0, init+delimiter.length());
    }
  }while((init = path.find(delimiter)) != std::string::npos);
  return outPath;
}

sop::files::Filesystem::Filesystem(sop::logger::Logger* logger) :
  logger(logger)
{
  this->logger->logFiles(5, "Initilizing filesystem");
  this->logger->logFiles(4, "Setting free spaces and inicilizing structures");
  for(uint32_t i=1; i < sop::files::ConstEV::numOfBlocks; i++)
  {
    this->freeSpace.push_back(i);
    this->dataBlocks[i] = 0;
  }
  std::sort(this->freeSpace.begin(), this->freeSpace.end());
  this->dataBlocks[0] = new sop::files::Inode(true, 0,0);
  this->logger->logFiles(6, "Initialization successful");
}

sop::files::Filesystem::Filesystem(sop::logger::Logger* logger, std::string diskFileName) :
  logger(logger)
{
}

sop::files::Filesystem::~Filesystem()
{
  this->logger->logFiles(6, "Destroing filesystem");
}

// Files
sop::files::File* sop::files::Filesystem::openFile(pid_t* PID, std::vector<std::string> path, char openMode)
{
  File* out = this->seek(0, path);
  if(out != 0)
  {
    out->setMode(openMode);
    this->openedFilesList.push_back(out);
    return out;
  }
  return 0;
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

void sop::files::Filesystem::createFile(pid_t* PID, std::vector<std::string> path)
{
  this->logger->logFiles(3, "File creation initilized");
  uint32_t iterator = 0;
  File* returned = seek(0, path);
  this->logger->logFiles(3, "Searching directory tree");
  if(returned != 0)
  {
    std::cout<<path.at(path.size()-1)<<" already exist!"<<std::endl;
    return;
  }
  if(this->currentDir.blockRoute.size())
  {
    iterator = this->currentDir.blockRoute.back();
  }
  if(path.size() > 1)
  {
    std::vector<std::string> tmp(path.begin(), path.end()-1);
    returned = seek(0, tmp);
    if(returned == 0)
    {
      std::cout<<"Directory not found! Create the directory first!"<<std::endl;
      return;
    }
    iterator = returned->getBlockAddr();
  }
  this->logger->logFiles(3, "Setting initial params");
  uid_t uid = this->dataBlocks[iterator]->getUID();
  gid_t gid = this->dataBlocks[iterator]->getGID();
  bool writePermission = 1; // sop::user::ask for write permission
  this->logger->logFiles(3, "Checking for permission");
  if(writePermission)
  {
    uint32_t reserveAddress = this->freeSpace.at(0);
    this->freeSpace.erase(this->freeSpace.begin());
    this->dataBlocks[iterator]->addInDir(path.at(path.size()-1), reserveAddress);
    this->dataBlocks[reserveAddress] = new Inode(false, uid, gid);
  }
  else
  {
    this->logger->logFiles(2, "File not created");
    std::cout<<"touch: cannot create "<<path.at(path.size()-1)<<std::endl;
    return;
  }
  this->logger->logFiles(5, "File has been created");
  std::cout<<"touch: "<<path.at(path.size()-1)<<" has been created!"<<std::endl;
}

void sop::files::Filesystem::closeFile(File* fileHandler)
{
  this->logger->logFiles(3, "Closing file "+fileHandler->getFileName());
  this->openedFilesList.remove(fileHandler);
  delete fileHandler;
  this->logger->logFiles(6, "File closed");
}

void sop::files::Filesystem::removeFile(pid_t* PID, std::vector<std::string> path)
{
  this->logger->logFiles(3, "File remove initilized");
  sop::files::File* fh = seek(0, path);
  std::vector<std::string> tmp(path);
  tmp.pop_back();  
  this->logger->logFiles(3, "Removing file");
  fh->removeFile(&this->freeSpace);
  if(tmp.size())
  {
    fh = seek(0, tmp);
    this->dataBlocks[fh->getBlockAddr()]->removeFromDir(path.back());
  }
  else
  {
    this->dataBlocks[this->getCurrentPathIterator()]->removeFromDir(path.at(0));
  }
  this->logger->logFiles(3, "File removed");
}

void sop::files::Filesystem::moveFile(pid_t* PID, std::string fileName, std::string newDirectory)
{
  std::cout<<"Not yet implemented"<<std::endl;
}

void sop::files::Filesystem::writeToFile(File* fileHandler, std::string data)
{
  this->logger->logFiles(3, "Write to file initilized");
  if(!this->dataBlocks[fileHandler->getBlockAddr()]->getIsDirectory())
  {
    this->dataBlocks[fileHandler->getBlockAddr()]->writeToFile(data, &this->freeSpace, &this->dataBlocks);
  }
  else
  {
    this->logger->logFiles(2, "Is a directory");
    std::cout<<fileHandler->getFileName()<<" is a directory!"<<std::endl;
    return;
  }
  this->logger->logFiles(3, "File written");
}

sop::files::File* sop::files::Filesystem::seek(pid_t* PID, std::vector<std::string> path)
{
  this->logger->logFiles(3, "Seek initilized");
  if(!path.size())
  {
    return 0;
  }
  std::vector<uint32_t> blockPath;
  uint32_t currentDir = 0;
  std::string filename = path[0];
  if(path.size() == 1 && path[0] == "/")
  {
    this->logger->logFiles(3, "Seek: root tree discovered");
    return new sop::files::File(PID, currentDir, currentDir, &this->dataBlocks);
  }
  if(path.size() > 1 && path[0] == "/")
  {
    this->logger->logFiles(3, "Seek: root tree discovered");
    blockPath.push_back(0);
    path.erase(path.begin());
  }
  else
  {
    this->logger->logFiles(3, "Seek: checking current directory");
    if(this->currentDir.blockRoute.size())
    {
      currentDir = this->currentDir.blockRoute.back();
    }
  }
  while(path.size() > 1)
  {
    if(this->dataBlocks.at(currentDir)->getAddress(path[0]))
    {
      currentDir = this->dataBlocks.at(currentDir)->getAddress(path[0]);
      filename = path[0];
      path.erase(path.begin());
    }
    else
    {
      this->logger->logFiles(2, "Seek: not found");
      std::cout<<filename<<" not found!"<<std::endl;
      return 0;
    }
  }
  if(this->dataBlocks.at(currentDir)->getAddress(filename))
  {
    this->logger->logFiles(3, "Seek: was found");
    this->logger->logFiles(3, "Search successful");
    return new sop::files::File(PID, currentDir, this->dataBlocks.at(currentDir)->getAddress(filename), &this->dataBlocks);
  }
  else
  {
    this->logger->logFiles(2, "Seek: not found");
    std::cout<<filename<<" not found!"<<std::endl;
    return 0;
  }
}
    
/* 
  Directories
*/
std::string sop::files::Filesystem::getCurrentDir()
{
  this->logger->logFiles(3, "Getting current dir");
  if(this->currentDir.path.size())
  {
    return this->currentDir.path.at(this->currentDir.path.size()-1);
  }
  return "/";
}

std::string sop::files::Filesystem::getCurrentPath()
{
  this->logger->logFiles(3, "Getting current path");
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

void sop::files::Filesystem::changeDirectory(pid_t* PID, std::vector<std::string> path)
{
  this->logger->logFiles(3, "Changing directory initilized");
  if(path.size() == 1)
  {
    this->logger->logFiles(3, "Cd: root tree found");
    if(path[0] == "/")
    {
      this->currentDir.blockRoute.clear();
      this->currentDir.path.clear();
    }
    else
    {
      uint32_t temporary = this->dataBlocks[0]->getAddress(path[0]);
      if(this->currentDir.blockRoute.size())
      {
        temporary = this->dataBlocks[this->currentDir.blockRoute.back()]->getAddress(path[0]);
      }
      if(this->dataBlocks[temporary]->getIsDirectory())
      {
        this->currentDir.blockRoute.push_back(temporary);
        this->currentDir.path.push_back(path[0]);
      }
      else
      {
        this->logger->logFiles(2, "Cd: Not a directory");
        std::cout<<"Not a directory!"<<std::endl;
        return;
      }
    }
  }
  else
  {
    uint32_t iter = 0;
    if(path[0] == "/")
    {
      this->currentDir.blockRoute.clear();
      this->currentDir.path.clear();
      iter = 1;
    }
    sop::files::File* fh;
    for(uint32_t i = iter; i<path.size()-1; i++)
    {
      std::vector<std::string> tmp(path.begin(), path.begin()+i);
      fh = seek(0, tmp);
      if(fh)
      {
        this->currentDir.blockRoute.push_back(fh->getBlockAddr());
        this->currentDir.path.push_back(path[i]);
      }
      else
      {
        this->logger->logFiles(2, "Cd: Not found");
        std::cout<<"Directory not found!"<<std::endl;
        break;
      }
    }
  }
}

void sop::files::Filesystem::changeDirectoryUp()
{
  this->logger->logFiles(3, "Cd: Went to parent directory");
  if(this->currentDir.blockRoute.size() > 0)
  {
    this->currentDir.blockRoute.pop_back();
    this->currentDir.path.pop_back();
  }
}

void sop::files::Filesystem::createDirectory(pid_t* PID, std::vector<std::string> path) // use temporary current Directory structure
{
  this->logger->logFiles(3, "Creating directory initilized");
  uint32_t iterator = 0;
  File* returned = seek(0, path);
  if(returned != 0 && returned->getBlockAddr())
  {
    this->logger->logFiles(2, "Directory already exist");
    std::cout<<path.at(path.size()-1)<<" already exist!"<<std::endl;
    return;
  }
  if(this->currentDir.blockRoute.size())
  {
    iterator = this->currentDir.blockRoute.back();
  }
  if(path.size() > 1)
  {
    std::vector<std::string> tmp(&path[0], &path[path.size()-2]);
    returned = seek(0, tmp);
    if(returned == 0)
    {
      std::cout<<"Directory not found! Create the directory first!"<<std::endl;
      return;
    }
    iterator = returned->getBlockAddr();
  }
  this->logger->logFiles(3, "Setting values");
  uid_t uid = this->dataBlocks[iterator]->getUID();
  gid_t gid = this->dataBlocks[iterator]->getGID();
  bool writePermission = 1; // sop::user::ask for write permission
  if(writePermission)
  {
    uint32_t reserveAddress = this->freeSpace.at(0);
    this->freeSpace.erase(this->freeSpace.begin());
    this->dataBlocks[iterator]->addInDir(path.at(path.size()-1), reserveAddress);
    this->dataBlocks[reserveAddress] = new Inode(true, uid, gid);
  }
  else
  {
    this->logger->logFiles(2, "Cannot create directory");
    std::cout<<"mkdir: cannot create "<<path.at(path.size()-1)<<std::endl;
    return;
  }
  this->logger->logFiles(4, "Creating directory successful");
  std::cout<<"mkdir: "<<path.at(path.size()-1)<<" has been created!"<<std::endl;
}

void sop::files::Filesystem::removeDirectory(pid_t* PID, std::vector<std::string> path)
{
  this->logger->logFiles(3, "Removing directory initialization");
  uint32_t iterator = 0;
  File* returned = seek(0, path);
  if(returned == 0)
  {
    std::cout<<path.at(path.size()-1)<<" doesn't exist!"<<std::endl;
    return;
  }
  if(path.size() > 1)
  {
    returned = seek(0, path);
    if(returned == 0)
    {
      std::cout<<"Directory not found!"<<std::endl;
      return;
    }
    iterator = returned->getBlockAddr();
  }
  bool writePermission = 1; // sop::user::ask for write permission
  this->logger->logFiles(3, "Asking for permission");
  if(writePermission)
  {
    if(this->dataBlocks[iterator]->getIsDirectory())
    {
      uint32_t tmp = iterator;
      iterator = this->dataBlocks[iterator]->getAddress(path.at(path.size()-1));
      this->dataBlocks[iterator]->removeDir(&this->freeSpace, &this->dataBlocks);
      this->dataBlocks[tmp]->removeFromDir(path.at(path.size()-1));
    }
    else
    {
      std::cout<<"Not a directory!"<<std::endl;
    }
  }
  else
  {
    this->logger->logFiles(2, "No permission. Removal process stoped");
    std::cout<<"rmdir: cannot remove "<<path.at(path.size()-1)<<"! No permission!"<<std::endl;
    return;
  }
  this->logger->logFiles(6, "Directory removal successful");
  std::cout<<"rmdir: "<<path.at(path.size()-1)<<" has been removed!"<<std::endl;
}

/*
  Overall
*/
std::vector<std::string> sop::files::Filesystem::list()
{
  this->logger->logFiles(3, "Initilizing listing");
  uint32_t iterator = 0;
  if(this->currentDir.blockRoute.size())
  {
    iterator = this->currentDir.blockRoute.back();
  }
  this->logger->logFiles(6, "Listing successful");
  return this->dataBlocks[iterator]->listDir();
}

/*
  Handlers
*/
void sop::files::Filesystem::changeDirectoryHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "CD handler initialization");
  if(params.size() > 1)
  {
    if(params.at(1) == "..")
    {
      this->changeDirectoryUp();
    }
    else if(params.at(1) == "/")
    {
      std::vector<std::string> root;
      root.push_back("/");
      this->changeDirectory(new pid_t(0),root); 
    }
    else
    {
      this->changeDirectory(new pid_t(0), getPathFromParam(params[1])); 
    }
  }
  else
  {
    this->logger->logFiles(6, "CD helper");
    std::cout<<"cd - changes directory to another"<<std::endl;
  }
}

// WRITE
void sop::files::Filesystem::moveHandler(const std::vector<const std::string> & params)
{
  std::cout<<"Not yet implemented"<<std::endl;
}

void sop::files::Filesystem::removeFileHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "Remove file handler initialization");
  auto param = params;
  param.erase(param.begin());
  for(auto data : param)
  {
    auto path = getPathFromParam(data);
    this->removeFile(0, path); //TEST get current pid
  }
}

void sop::files::Filesystem::nanoHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "File editor");
  std::cout<<"Loading nano..."<<std::endl<<"Nano not yet supported"<<std::endl;
}

void sop::files::Filesystem::createFileHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "Create file handler initialization");
  auto param(params);
  param.erase(param.begin());
  if(!param.size())
  {
    std::cout<<"touch - creates a file"<<std::endl;
  }
  for(auto data : param)
  {
    auto path = getPathFromParam(data);
    pid_t* PID = 0;
    this->createFile(PID, path); // TEST check the pid of actually logged user
  }
}

void sop::files::Filesystem::createDirectoryHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "Create directory handler initalization");
  auto param(params);
  param.erase(param.begin());
  if(!param.size())
  {
    std::cout<<"mkdir - makes a directory"<<std::endl;
  }
  for(std::string data : param)
  {
    std::cout<<data<<std::endl;
    auto path = getPathFromParam(data);
    this->createDirectory(0, path);
  }
}

void sop::files::Filesystem::removeDirectoryHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "Remove directory handler initalization");
  auto param(params);
  param.erase(param.begin());
  if(!param.size())
  {
    std::cout<<"rmdir - removes a directory"<<std::endl;
  }
  for(auto data : param)
  {
    auto path = getPathFromParam(data);
    this->removeDirectory(0, path);
  }
}

void sop::files::Filesystem::listHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "List handler initalization");
  std::vector<std::string> x = this->list();
  if(x.size())
  {
    std::cout<<"<   drwx   >\t<  user  >\t<size>\t< filename >"<<std::endl;
    for(auto data : x)
    {
      std::cout<<"\t\t\t"<<data<<std::endl;
    }
  }
  else
  {
    std::cout<<"Total: 0"<<std::endl;
  }
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
        std::cout<<"^|"; // raw data
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

void sop::files::Filesystem::printDiskTree(uint32_t depth)
{
  this->logger->logFiles(6, "Printing disk tree");
  std::cout<<"Loading DiskTree"<<std::endl;
  std::cout<<"DiskTree: Not yet implemented"<<std::endl;
}

void sop::files::Filesystem::statHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(6, "Printing disk statistics");
  this->printStats();
  this->printDisk(16);
}

// REWRITE!!!
void sop::files::Filesystem::echoHandler(const std::vector<const std::string> & params){}
/*{
  switch(params.size())
  {
  case 2:
    {
      std::cout<<params[1]<<std::endl;
      break;
    }
  case 4:
    {
      if(params[2] == std::string(">"))
      {
        sop::files::File* fh = this->seekForFile(0, params[3], &this->dataBlocks);
        if(fh != 0)
        {
          fh->writeToFile(params[1], &this->freeSpace);
          delete fh;
          std::cout<<params[1]<<" has been writen to "<<params[3]<<std::endl;
        }
        else
        {
          std::cout<<"File "<<params[3]<<" not found!"<<std::endl;
        }
      }
      else
      {
        std::cout<<"Regex not valid!"<<std::endl;
      }
      break;
    }
  default:
    {
      std::cout<<"echo - display a line of text"<<std::endl;
    }
  }
}
*/

void sop::files::Filesystem::test(const std::vector<const std::string> & params)
{
  std::vector<std::string> fileName;
  fileName.push_back("abc");
  pid_t* PID = 0;
  this->createFile(PID, fileName);

  sop::files::File* fh = this->openFile(PID, fileName, 'r');
  std::string temp;
  std::cin>>temp;
  fh->writeToFile(temp, &this->freeSpace);
  this->closeFile(fh);
}

uint32_t sop::files::Filesystem::getCurrentPathIterator()
{
  if(this->currentDir.blockRoute.size() > 0)
  {
    return this->currentDir.blockRoute.back();
  }
  return 0;
}