#include <string>
#include <iostream>
#include <conio.h>
#include <math.h>
#include <algorithm>
#include <cstdint>
#include <Windows.h>
#include <boost\make_shared.hpp>
#include <boost\make_shared.hpp>
#include ".\sop\files\filesystem.h"
#include ".\sop\files\file.h"
#include ".\sop\files\inode.h"
#include ".\sop\files\data.h"
#include ".\sop\logger\logger.h"
#include ".\sop\files\serialize.h"
#include "temporary.h"

void clearConsole()
{
  COORD topLeft={0,0};
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen;
  DWORD written;

  GetConsoleScreenBufferInfo(console, &screen);
  FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
  FillConsoleOutputAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
  SetConsoleCursorPosition(console, topLeft);
}

std::vector<std::string> getPathFromParam(std::string path)
{
  std::string delimiter = "/";
  std::vector<std::string> outPath;
  size_t init = 0;
  if(path.length() <= delimiter.length() && path == delimiter)
  {
    outPath.push_back(delimiter);
    return outPath;
  }
  while((init = path.find(delimiter)) != std::string::npos)
  {
    outPath.push_back(path.substr(0,init));
    std::cout<<path.substr(0,init)<<std::endl;
    path.erase(0, init+delimiter.length());
  }
  outPath.push_back(path);
  return outPath;
}

sop::files::Filesystem::Filesystem(sop::logger::Logger* logger, std::string diskFileName) :
  logger(logger)
{
  this->logger->logFiles(6, "Initilizing filesystem");
  this->logger->logFiles(3, "Restore filesystem initilized");
  for(uint32_t i=0; i < sop::files::ConstEV::numOfBlocks; i++)
  {
    //this->freeSpace.push_back(i);
    this->dataBlocks[i] = 0;
  }
  this->serialize = new sop::files::Serialize(this, diskFileName, this->logger);
  this->serialize->read();
  if(this->dataBlocks[0] == 0)
  {
    this->logger->logFiles(1, "Filesystem corrupted");
    std::cout<<"[######] Filesystem corrupted"<<std::endl<<"Press a key to exit . . ."<<std::endl;
    this->format();
    this->serialize->save();
    _getch();
    exit(-1);
  }
}

sop::files::Filesystem::~Filesystem()
{
  this->logger->logFiles(6, "Destroing filesystem");
  this->serialize->save();
}

// Files
sop::files::File* sop::files::Filesystem::openFile(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path, std::string openMode)
{
  this->serialize->read();
  // ToDo MODE DEPENDENCY
  if(path.size()>1)
  {
    std::vector<uint32_t> currentDirBlocks;
    std::vector<std::string> currentDirPath;
    std::vector<std::string> root;
    root.push_back("/");
    
    for(uint32_t i=0; i<this->currentDir.blockRoute.size(); i++)
    {
      currentDirBlocks.push_back(this->currentDir.blockRoute[i]);
      currentDirPath.push_back(this->currentDir.path[i]);
    }
    this->currentDir.path.clear();
    this->currentDir.blockRoute.clear();
    std::vector<std::string> tmp(path);
    path.clear();
    path.push_back(tmp.back());
    tmp.pop_back();
    this->changeDirectory(PID, tmp);
    File* out = this->seek(PID, path);
    sop::users::PermissionsManager pm;
    sop::users::permission_t perm;
    switch(openMode[0])
    {
    case 'w':
      perm = 2;
      break;
    case 'r':
      perm = 4;
      break;
    case 'x':
      perm = 1;
      break;
    }
    if(out != 0 && pm.hasPermission(out->getInode(), PID, perm))
    {
      out->setMode((char)openMode[0]);
      out->setFilename(path.at(path.size()-1));
      this->openedFilesList.push_back(out);
    }
    this->currentDir.path.clear();
    this->currentDir.blockRoute.clear();
    for(uint32_t i=0; i<currentDirBlocks.size(); i++)
    {
      this->currentDir.path.push_back(currentDirPath[i]);
      this->currentDir.blockRoute.push_back(currentDirBlocks[i]);
    }
    return out;
  }
  //change the path to appropriate
  if(path.size() == 1)
  {
    File* out = this->seek(PID, path);
    sop::users::PermissionsManager pm;
    sop::users::permission_t perm;
    switch(openMode[0])
    {
    case 'w':
      perm = 2;
      break;
    case 'r':
      perm = 4;
      break;
    case 'x':
      perm = 1;
      break;
    }
    if(out != 0 && pm.hasPermission(out->getInode(),PID, perm))
    {
      out->setMode((char)openMode[0]);
      out->setFilename(path.at(path.size()-1));
      this->openedFilesList.push_back(out);
    }
    return out;
  }
  return 0;
}

std::string sop::files::Filesystem::readFile(File* fileHandler)
{
  this->serialize->read();
  std::string out = "";
  std::vector<std::array<char, sop::files::ConstEV::blockSize>> data = fileHandler->getData();
  uint32_t size = fileHandler->getSize();
  for(uint32_t y=0; y<data.size(); y++)
  {
    for(uint32_t i=0; i<sop::files::ConstEV::blockSize; i++)
    {
      if(size > 0)
      {
        out+= data[y][i];
        size--;
      }
      else
      {
        break;
      }
    }
  }
  return out;
}

void sop::files::Filesystem::createFile(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path)
{
  this->serialize->read();
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
  sop::users::PermissionsManager pm;
  bool writePermission = pm.hasPermission(dynamic_cast<Inode*>(this->dataBlocks[iterator]), PID, 6); // sop::user::ask for write permission
  this->logger->logFiles(3, "Checking for permission");
  if(writePermission)
  {
    if(this->freeSpace.size() < 1)
    {
      std::cout<<"Insufficient memory"<<std::endl;
      this->logger->logFiles(2, "Insufficient memory");
      return;
    }
    uint32_t reserveAddress = this->freeSpace.at(0);
    this->freeSpace.erase(this->freeSpace.begin());
    this->dataBlocks[iterator]->addInDir(path.at(path.size()-1), reserveAddress);
    this->dataBlocks[reserveAddress] = new Inode(false, uid, gid, this->logger);
  }
  else
  {
    this->logger->logFiles(2, "File not created");
    std::cout<<"touch: cannot create "<<path.at(path.size()-1)<<std::endl;
    return;
  }
  this->logger->logFiles(5, "File has been created");
  std::cout<<"touch: "<<path.at(path.size()-1)<<" has been created!"<<std::endl;
  this->serialize->save();
}

void sop::files::Filesystem::closeFile(File* fileHandler)
{
  this->logger->logFiles(3, "Closing file "+fileHandler->getFileName());
  this->openedFilesList.remove(fileHandler);
  //delete fileHandler;
  this->logger->logFiles(6, "File closed");
  this->serialize->save();
}

void sop::files::Filesystem::removeFile(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path)
{
  try
  {
    this->serialize->read();
    this->logger->logFiles(3, "File remove initilized");
    sop::files::File* fh = seek(0, path);
    std::vector<std::string> tmp(path);
    tmp.pop_back();  
    this->logger->logFiles(3, "Removing file");
    sop::users::PermissionsManager pm;
    if(fh != 0 && !fh->getInode()->getIsDirectory())
    {
      if(pm.hasPermission(fh->getInode(), PID, 6))
      {
        fh->removeFile(&this->freeSpace);
        if(tmp.size())
        {
          fh = seek(0, tmp);
          this->dataBlocks.at(fh->getBlockAddr())->removeFromDir(path.back());
        }
        else
        {
          uint32_t tmp = this->getCurrentPathIterator();
          if(tmp >= 0 && tmp<sop::files::ConstEV::numOfBlocks && this->dataBlocks[tmp] != 0)
          {
            this->dataBlocks.at(this->getCurrentPathIterator())->removeFromDir(path.at(0));
          }
        }
        this->logger->logFiles(3, "File removed");
        this->serialize->save();
      }
      else
      {
        std::cout<<"No permission!"<<std::endl;
        this->logger->logFiles(3, "No permission to remove file");
        return;
      }
    }
    else
    {
      std::cout<<"Is a directory or doesn't exist!"<<std::endl;
      this->logger->logFiles(3, "rm: Is a directory or doesn't exist");
    }
  }
  catch(...)
  {
  }
}

void sop::files::Filesystem::moveFile(boost::shared_ptr<sop::process::Process> PID, std::string fileName, std::string newDirectory)
{
  std::cout<<"Not yet implemented"<<std::endl;
}

void sop::files::Filesystem::writeToFile(File* fileHandler, std::string data)
{
  sop::users::PermissionsManager pm;
  if(pm.hasPermission(fileHandler->getInode(), fileHandler->getPID(), 6))
  {
    this->serialize->read();
    this->logger->logFiles(3, "Write to file initilized");
    this->dataBlocks[fileHandler->getBlockAddr()]->writeToFile(data, &this->freeSpace, &this->dataBlocks);
    this->logger->logFiles(3, "File written");
    this->serialize->save();
  }
}

sop::files::File* sop::files::Filesystem::seek(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path)
{
  this->serialize->read();
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
    sop::files::File* fh = new sop::files::File(PID, currentDir, currentDir, &this->dataBlocks, this->logger);
    return fh;
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
  if(this->dataBlocks[currentDir] != 0 && this->dataBlocks.at(currentDir)->getAddress(filename))
  {
    this->logger->logFiles(3, "Seek: was found");
    this->logger->logFiles(3, "Search successful");
    sop::files::File* fh = new sop::files::File(PID, currentDir, this->dataBlocks.at(currentDir)->getAddress(filename), &this->dataBlocks, this->logger);
    fh->setFilename(filename);
    return fh;
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

void sop::files::Filesystem::changeDirectory(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path)
{ 
  this->serialize->read();
  uint32_t iter = 0;
  this->logger->logFiles(3, "Changing directory initilized");
  if(path[0] == "/")
  {
    this->logger->logFiles(3, "Cd: root tree found");
    this->currentDir.blockRoute.clear();
    this->currentDir.path.clear();
    iter = 1;
    if(path.size() == 1)
    {
      return;
    }
    path.erase(path.begin());
  }
  if(path.size() == 1)
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
  else
  {
    std::vector<std::string> tmpPath;
    for(uint32_t i=iter; i<path.size()-1; i++)
    {
      tmpPath.push_back(path[i]);
      this->changeDirectory(PID, tmpPath);
      tmpPath.pop_back();
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

void sop::files::Filesystem::createDirectory(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path) // use temporary current Directory structure
{
  this->serialize->read();
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
  sop::users::PermissionsManager pm;
  if(pm.hasPermission(dynamic_cast<Inode*>(this->dataBlocks[iterator]), PID, 6))
  {
    if(this->freeSpace.size() < 1)
    {
      std::cout<<"Insufficient memory"<<std::endl;
      this->logger->logFiles(2, "Insufficient memory");
      return;
    }
    uint32_t reserveAddress = this->freeSpace.at(0);
    this->freeSpace.erase(this->freeSpace.begin());
    this->dataBlocks[iterator]->addInDir(path.at(path.size()-1), reserveAddress);
    this->dataBlocks[reserveAddress] = new Inode(true, uid, gid, this->logger);
  }
  else
  {
    this->logger->logFiles(2, "No permission!");
    std::cout<<"mkdir: no permission"<<std::endl;
    return;
  }
  this->logger->logFiles(6, "Creating directory successful");
  std::cout<<"mkdir: "<<path.at(path.size()-1)<<" has been created!"<<std::endl;
  this->serialize->save();
}

void sop::files::Filesystem::removeDirectory(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path)
{
  this->serialize->read();
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
  sop::users::PermissionsManager pm;
  if(pm.hasPermission(dynamic_cast<Inode*>(this->dataBlocks[iterator]), PID, 6))
  {
    if(this->dataBlocks[iterator]->getIsDirectory())
    {
      uint32_t tmp = iterator;
      try
      {
        iterator = this->dataBlocks[iterator]->getAddress(path.at(path.size()-1));
        this->dataBlocks[iterator]->removeDir(&this->freeSpace, &this->dataBlocks);
        this->dataBlocks[tmp]->removeFromDir(path.at(path.size()-1));
      }
      catch(std::string)
      {
        this->logger->logFiles(2, "Failed in removing");
        std::cout<<"Removal failed"<<std::endl;
        return;
      }
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
  this->serialize->save();
}

/*
  Overall
*/
std::vector<sop::files::dirList> sop::files::Filesystem::list()
{
  this->serialize->read();
  this->logger->logFiles(3, "Initilizing listing");
  uint32_t iterator = 0;
  if(this->currentDir.blockRoute.size())
  {
    iterator = this->currentDir.blockRoute.back();
  }
  this->logger->logFiles(6, "Listing successful");
  if(this->dataBlocks[iterator] != 0)
  {
    return this->dataBlocks[iterator]->listDir(&this->dataBlocks);
  }
  return *new std::vector<sop::files::dirList>;
}

/*
  Handlers
*/
void sop::files::Filesystem::changeDirectoryHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "Change directory handler initialization");
  if(params.size()>1 && params[1]=="-h")
  {
    std::cout<<"cd - changes the directory to another"<<std::endl;
    return;
  }
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
      this->changeDirectory(boost::make_shared<sop::process::Process>(0),root); 
    }
    else
    {
      this->changeDirectory(boost::make_shared<sop::process::Process>(0), getPathFromParam(params[1])); 
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
  if(params.size()>1 && params[1] == "-h")
  {
    std::cout<<"mv - moves the file/directory to another"<<std::endl;
    return;
  }
  std::cout<<"Not yet implemented"<<std::endl;
}

void sop::files::Filesystem::removeFileHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "Remove file handler initialization");
  if(params.size()>1 && params[1] == "-h")
  {
    std::cout<<"rm - removes file"<<std::endl;
    return;
  }
  auto param = params;
  param.erase(param.begin());
  for(auto data : param)
  {
    auto path = getPathFromParam(data);
    if(path.size()>1)
    {
      std::cout<<"Not supported. Try to use one one level path"<<std::endl;
    }
    else
    {
      this->removeFile(0, path); //TEST get current pid ToDo
    }
  }
}

void sop::files::Filesystem::vi(sop::files::File* fp)
{
  std::cout<<std::endl<<std::endl;
  std::cout<<"Filename: "<<fp->getFileName()<<std::endl;
  std::cout<<":r - read, :w - write, :q - quit, :i - input mode, :c - clear"<<std::endl;
  enum mode
  {
    INPUT = 1,
    READ = 2,
    WRITE = 3,
    WAIT = 4,
    CLEAR = 5,
    QUIT = 0
  };
  mode modeVal = WAIT;
  std::string output = "";
  bool exit = true;
  while(modeVal)
  {
    switch(modeVal)
    {
    case INPUT:
      this->logger->logFiles(3, "Vi: reading data from file");
      output = this->readFile(fp);
      std::cout<<output<<std::endl;
      char tmp;
      while((tmp = _getch()) !=27)
      {
        output += tmp;
        //this->logger->logFiles(3, "Vi: reading characters: "+std::to_string(tmp));
        _putch(tmp);
        if(tmp == 13)
        {
          std::cout<<std::endl;
        }
      }
      modeVal = WAIT;
      break;
    case READ:
      std::cout<<std::endl<<std::endl;
      this->logger->logFiles(3, "Vi: reading data from file");
      output = this->readFile(fp);
      std::cout<<output<<std::endl;
      std::cout<<"Data read"<<std::endl;
      modeVal = WAIT;
      break;
    case WRITE:
      while(output.find("\r") != std::string::npos)
      {
        output.replace(output.find("\r"), 1, "\n");
      }
      this->writeToFile(fp, output);
      this->logger->logFiles(3, "Vi: data written");
      std::cout<<"Data written"<<std::endl;
      modeVal = WAIT;
      break;
    case WAIT:
      char tmp1, tmp2;
      do
      {
        std::cout<<"\rChoose mode ";
        tmp1 = _getch();
        _putch(tmp1);
        if(tmp1 == 27)
        {
          std::cout<<"\rChoose mode           ";
          continue;
        }
        if((tmp1) == ':')
        {
          if(tmp1 == 27)
          {
            std::cout<<"\rChoose mode           ";
            continue;
          }
          tmp2 = _getch();
          _putch(tmp2);
          exit = true;
          switch(tmp2)
          {
          case 'i':
            modeVal = INPUT;
            break;
          case 'r':
            modeVal = READ;
            break;
          case 'w':
            modeVal = WRITE;
            break;
          case'q':
            modeVal = QUIT;
            break;
          case 'c':
            modeVal = CLEAR;
            break;
          default:
            exit = false;
          }
        }
        else
        {
          exit = false;
        }
      } while(!exit);
      std::cout<<std::endl;
      break;
    case CLEAR:
      output.clear();
      std::cout<<output<<std::endl;
      modeVal = WAIT;
      break;
    default:
      std::cout<<"Error: not known"<<std::endl;
      this->logger->logFiles(2, "Error, not known");
      break;
    }
  }
}

void sop::files::Filesystem::viHandler(const std::vector<const std::string> & params)
{
  this->serialize->read();
  if(params.size()>1 && params[1] == "-h")
  {
    std::cout<<"vi - test-fitted file editor. Allows to edit files, guess You not"<<std::endl;
    return;
  }
  clearConsole();
  this->logger->logFiles(3, "File editor");
  std::cout<<"Loading vi..."<<std::endl;
  //std::cout<<"Vi not yet supported"<<std::endl;
  this->serialize->read();
  if(params.size() == 2)
  {
    boost::shared_ptr<sop::process::Process> PID = 0; // ToDo current pid
    sop::files::File* fh = this->openFile(PID, getPathFromParam(params[1]), "w");
    if(fh != 0)
    {
      this->vi(fh);
      this->closeFile(fh);
    }
  }
  //this->serialize->save();
}

void sop::files::Filesystem::createFileHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "Create file handler initialization");
  if(params.size()>1 && params[0] == "-h")
  {
    std::cout<<"touch - creates file"<<std::endl;
    return;
  }
  auto param(params);
  param.erase(param.begin());
  if(!param.size())
  {
    std::cout<<"touch - creates a file"<<std::endl;
  }
  for(auto data : param)
  {
    auto path = getPathFromParam(data);
    if(path.size() == 1)
    {
      boost::shared_ptr<sop::process::Process> PID = 0;
      this->createFile(PID, path); // TEST check the pid of actually logged user ToDo
    }
    else
    {
      std::cout<<"Not supported. Try to use one one level path"<<std::endl;
    }
  }
}

void sop::files::Filesystem::createDirectoryHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "Create directory handler initalization");
  if(params.size()>1 && params[1] == "-h")
  {
    std::cout<<"mkdir - creates directory"<<std::endl;
    return;
  }
  auto param(params);
  param.erase(param.begin());
  if(!param.size())
  {
    std::cout<<"mkdir - makes a directory"<<std::endl;
  }
  for(std::string data : param)
  {
    auto path = getPathFromParam(data);
    if(path.size()>1)
    {
      std::cout<<"Not supported. Try to use one one level path"<<std::endl;
    }
    else
    {
      if(this->freeSpace.size()>1)
      {
        this->createDirectory(0, path);
      }
      else
      {
        std::cout<<"Insufficient memory"<<std::endl;
      }
    }
  }
}

void sop::files::Filesystem::removeDirectoryHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "Remove directory handler initalization");
  if(params.size()>1 && params[1] == "-h")
  {
    std::cout<<"rmdir - removes directory"<<std::endl;
    return;
  }
  auto param(params);
  param.erase(param.begin());
  if(!param.size())
  {
    std::cout<<"rmdir - removes a directory"<<std::endl;
  }
  for(auto data : param)
  {
    auto path = getPathFromParam(data);
    if(path.size()>1)
    {
      std::cout<<"Not supported. Try to use one one level path"<<std::endl;
    }
    else
    {
      this->removeDirectory(0, path);
    }
  }
}

void sop::files::Filesystem::listHandler(const std::vector<const std::string> & params)
{
  this->logger->logFiles(3, "List handler initalization");
  if(params.size()>1 && params[1] == "-h")
  {
    std::cout<<"ll - lists files and directories in current directory"<<std::endl;
    return;
  }
  std::vector<sop::files::dirList> x = this->list();
  uint32_t size = x.size();
  if(x.size())
  {
    std::cout<<"<   drwx   >\t<size>\t< filename >"<<std::endl;
    for(auto data : x)
    {
      std::cout<<" "<<data.drwx<<"\t"<<"  "<<data.size<<"\t"<<"  "<<data.name<<std::endl;
    }
    
  }
  std::cout<<"_________"<<std::endl;
  std::cout<<"Total: "<<std::to_string(size)<<std::endl;
}

void sop::files::Filesystem::catHandler(const std::vector< const std::string>& params)
{
  this->logger->logFiles(3, "Cat handler initalization");
  if(params.size()>1 && params[1] == "-h")
  {
    std::cout<<"cat - prints inside of a file"<<std::endl;
    return;
  }
  boost::shared_ptr<sop::process::Process> PID = 0;
  if(params.size()>1)
  {
    for(uint32_t i=1; i<params.size(); i++)
    {
      std::vector<std::string> tmp = getPathFromParam(params[i]);
      sop::files::File* fp = this->openFile(PID,tmp, "r");
      if(fp == 0)
      {
        this->logger->logFiles(2, "File not found");
        std::cout<<"File not found"<<std::endl;
        return;
      }
      else
      {
        std::cout<<this->readFile(fp)<<std::endl;
      }
      closeFile(fp);
    }
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
  if(parts == 0)
  {
    return;
  }
  std::cout<<"   ";
  for(uint32_t tmp=0; tmp<parts; tmp++)
  {
    if(tmp < 10)
    {
      std::cout<<std::to_string(tmp);
    }
    else
    {
      std::cout<<static_cast<char>(tmp+55);
    }
    std::cout<<" ";
  }
  std::cout<<std::endl;
  for(uint32_t iterator=0; iterator < ceil(sop::files::ConstEV::numOfBlocks/parts); iterator++)
  {
    std::cout<<"  ";
    for(uint32_t tmp=0; tmp<(parts*2+1); tmp++)
    {
      std::cout<<"-";
    }
    if(iterator < 9)
    {
      std::cout<<std::endl<<std::to_string(iterator+1)<<" |";
    }
    else
    {
      std::cout<<std::endl<<std::to_string(iterator+1)<<"|";
    }
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
  std::cout<<"  ";
  for(uint32_t tmp=0; tmp<(parts*2+1); tmp++)
  {
    std::cout<<"-";
  }
  std::cout<<std::endl;
}

void sop::files::Filesystem::diskTree(uint32_t depth, std::vector<sop::files::dirList> root, std::string prelimiter)
{
  if(depth > 0)
  {
    if(root.size()>0)
    {
      for(uint32_t x=0; x<root.size(); x++)
      {
        std::cout<<prelimiter<<root[x].name<<std::endl;
        if(this->dataBlocks[root[x].block]->getIsDirectory())
        {
          this->diskTree(depth-1, this->dataBlocks[root[x].block]->listDir(&this->dataBlocks),prelimiter+"| ");
        }
      }
    }
    else return;
  }
  else return;
}

void sop::files::Filesystem::printDiskTree(uint32_t depth)
{
  this->logger->logFiles(6, "Printing disk tree");
  std::cout<<"Loading DiskTree..."<<std::endl;
  if(depth > 0)
  {
    std::vector<sop::files::dirList> root = this->dataBlocks[0]->listDir(&this->dataBlocks);
    if(root.size() > 0)
    {
      std::cout<<std::endl;
      this->diskTree(depth, root, " ");
    }
    else
    {
      std::cout<<"Directory is empty"<<std::endl;
    }
  }
  else
  {
    std::cout<<"DiskTree: digging deep inside your lungs is not possible"<<std::endl;
    this->logger->logFiles(2, "DiskTree: digging deep inside your lungs is not possible");
  }
}

void sop::files::Filesystem::printInodeBlock(uint32_t block)
{
  if(this->dataBlocks[block] != 0)
  {
    std::cout<<"Is directory: "<<this->dataBlocks[block]->getIsDirectory()<<std::endl;
    std::cout<<"UID: "<<this->dataBlocks[block]->getUID()<<std::endl;
    std::cout<<"GID: "<<this->dataBlocks[block]->getGID()<<std::endl;
    std::cout<<"RWX: "<<this->dataBlocks[block]->getPermissions().user<<this->dataBlocks[block]->getPermissions().group<<this->dataBlocks[block]->getPermissions().others<<std::endl;
    std::cout<<"Size: ";
    if(this->dataBlocks[block]->getIsDirectory())
    {
      std::vector<sop::files::dirList> listing = this->dataBlocks[block]->listDir(&this->dataBlocks);
      std::cout<<listing.size()<<std::endl;
    }
    else
    {
      std::cout<<this->dataBlocks[block]->getSize()<<std::endl;
    }
  }
  else
  {
    std::cout<<"Inode block not found!"<<std::endl;
    this->logger->logFiles(2, "Inode block not found!");
  }
}

void sop::files::Filesystem::printDataBlock(uint32_t block)
{
  if(this->dataBlocks[block] != 0)
  {
    if(this->dataBlocks[block]->getIsDirectory())
    {
      std::cout<<"This is a directory!"<<std::endl;
      this->logger->logFiles(2, "This is a directory");
    }
    else
    {
      std::array<char, sop::files::ConstEV::blockSize> data = this->dataBlocks[block]->getData_d();
      for(uint32_t i=0; i<sop::files::ConstEV::blockSize; i++)
      {
        std::cout<<data[i];
      }
      std::cout<<std::endl;
    }
  }
  else
  {
    std::cout<<"Data block not found!"<<std::endl;
    this->logger->logFiles(2, "Data block not found!");
  }
}

void sop::files::Filesystem::statHandler(const std::vector<const std::string> & params)
{
  std::vector<std::string> param;
  for(uint32_t i=0; i<params.size(); i++)
  {
    param.push_back(params[i]);
  }
  param.erase(param.begin());
  this->logger->logFiles(6, "Printing disk statistics");
  if(param.size()>0)
  {
    for(uint32_t i=0; i < param.size(); i++)
    {
      if(param[i] == "-h")
      {
        std::cout<<"DiskStat - prints disk based statistics"<<std::endl;
        std::cout<<"   -d <blockNr>\t prints data block if available"<<std::endl;
        std::cout<<"   -i <blockNr>\t prints inode data if available"<<std::endl;
        std::cout<<"   -s \t\t prints opened files, free spaces, current path..."<<std::endl;
        std::cout<<"   -t <depth>\t prints disk tree"<<std::endl;
        std::cout<<"   -f \t\t prints free spaces"<<std::endl;
        std::cout<<"   --disk \t prints disk blocks summary"<<std::endl;
        return;
      } 
      if(param[i] == "--disk")
      {
        if(param.size() > i+1)
        {
          this->printDisk(atoi(param[i+1].c_str()));
          i++;
        }
        else
        {
          std::cout<<"No parameter for disk print!"<<std::endl;
          this->logger->logFiles(2, "No parameter for disk print");
        }
      }
      if(param[i] == "-s")
      {
        this->printStats();
      }
      if(param[i] == "-f")
      {
        for(uint32_t i=0; i<(freeSpace.size()); i++)
        {
          std::cout<<this->freeSpace[i]<<"\t";
          if((i+1)%6 == 0 && i!=0)
          {
            std::cout<<std::endl;
          }
        }
        std::cout<<std::endl;
      }
      if(param[i] == "-i")
      {
        if(param.size() > i+1)
        {
          this->printInodeBlock(atoi(param[i+1].c_str()));
          i++;
        }
        else
        {
          std::cout<<"No parameter for Inode print!"<<std::endl;
          this->logger->logFiles(2, "No parameter for Inode print");
        }
      }
      if(param[i] == "-d")
      {
        if(param.size() > i+1)
        {
          this->printDataBlock(atoi(param[i+1].c_str()));
          i++;
        }
        else
        {
          std::cout<<"No parameter for data block print!"<<std::endl;
          this->logger->logFiles(2, "No parameter for data block print");
        }
      }
      if(param[i] == "-t")
      {
        if(param.size() > i+1)
        {
          this->printDiskTree(atoi(param[i+1].c_str()));
          i++;
        }
        else
        {
          std::cout<<"No parameter for disk tree print!"<<std::endl;
          this->logger->logFiles(2, "No parameter for disk tree print");
        }
      }
    }
  }
  else
  {
    this->printStats();
    std::cout<<std::endl;
    this->printDisk(16);
  }
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
  std::vector<std::string> test;
  test.push_back("abc");
  File* one = this->openFile(0, test, "w");
  File* two = this->openFile(0, test, "w");
  //std::cout<<"Currently no pending tests"<<std::endl;
}

uint32_t sop::files::Filesystem::getCurrentPathIterator()
{
  if(this->currentDir.blockRoute.size() > 0)
  {
    return this->currentDir.blockRoute.back();
  }
  return 0;
}

std::string sop::files::Filesystem::writeInode(uint32_t addr)
{
  std::string output = "dir="+std::to_string((uint32_t)this->dataBlocks[addr]->getIsDirectory())+"\n";
  output += "uid="+std::to_string(this->dataBlocks[addr]->getUID())+"\n";
  output += "gid="+std::to_string(this->dataBlocks[addr]->getGID())+"\n";
  output += "mod="+std::to_string(this->dataBlocks[addr]->getPermissions().user)+","+std::to_string(this->dataBlocks[addr]->getPermissions().group)+","+std::to_string(this->dataBlocks[addr]->getPermissions().others)+"\n";
  Inode* copy = dynamic_cast<Inode*>(this->dataBlocks[addr]);
  if(this->dataBlocks[addr]->getIsDirectory())
  {
    for(auto iter : copy->directory.inodesInside)
    {
      output += iter.first+"\t"+std::to_string(iter.second)+"\n";
    }
  }
  else
  {
    output += "direct=";
    for(uint32_t i=0; i<sop::files::ConstEV::directAddrBlock; i++)
    {
      output += std::to_string(copy->file.directBlockAddr[i]) + ",";
    }
    output += "\nindirect=";
    for(uint32_t i=0; i<copy->file.indirectBlockAddr.size(); i++)
    {
      output += std::to_string(copy->file.indirectBlockAddr[i]) + ",";
    }
    output +="\nsize="+std::to_string(copy->file.size)+"\n";
  }
  return output;
}

std::string sop::files::Filesystem::writeData(uint32_t addr)
{
  std::string output="";
  std::array<char, sop::files::ConstEV::blockSize> tmp = this->dataBlocks[addr]->getData_d();
  for(uint32_t i=0; i<sop::files::ConstEV::blockSize; i++)
  {
    output += tmp[i];
  }
  return output;
}

void sop::files::Filesystem::readInode(uint32_t addr,  std::vector<std::string> data)
{
  bool isDir;
  uint32_t UID, GID;
  //read
  bool valid = true;
  std::map<std::string, uint32_t> dirmap;
  std::vector<uint32_t> direct, indirect;
  uint32_t size;
  for(uint32_t i=0; i<4; i++)
  {
    if(!data[0][i] == "dir="[i]) valid = false;
    if(!data[1][i] == "uid="[i]) valid = false;
    if(!data[2][i] == "gid="[i]) valid = false;
  }
  if(valid)
  {
    if(data[0][4] == '0')
    {
      isDir = false;
    }
    else
    {
      isDir = true;
    }
    UID = atoi(data[1].substr(4,data[1].size()-5).c_str());
    GID = atoi(data[2].substr(4, data[2].size()-5).c_str());
  }
  else
  {
    return;
  }
  uint8_t puser, pgroup, pothers;
  if(data[3].find("mod=") != std::string::npos)
  {
    data[3].erase(data[3].begin(), data[3].begin()+4);
    puser = atoi(data[3].substr(0,1).c_str());
    data[3].erase(data[3].begin(), data[3].begin()+2);
    pgroup = atoi(data[3].substr(0,1).c_str());
    data[3].erase(data[3].begin(), data[3].begin()+2);
    pothers = atoi(data[3].substr(0,1).c_str());
  }
  if(isDir)
  {
    for(uint32_t i=4; i<data.size(); i++)
    {
      std::string a = data[i].substr(0, data[i].find("\t"));
      data[i].erase(0, data[i].find("\t")+1);
      std::string b = data[i];
      if(a != "")
      {
        dirmap[a] = atoi(b.c_str());
      }
    }
  }
  else
  {
    data[4].erase(data[4].find("direct="), 7);
    while(data[4].size())
    {
      direct.push_back(atoi(data[4].substr(0, data[4].find(",")).c_str()));
      data[4].erase(0, data[4].find(",")+1);
    }
    data[5].erase(data[5].find("indirect="), 9);
    while(data[5].size())
    {
      indirect.push_back(atoi(data[4].substr(0, data[5].find(",")).c_str()));
      data[5].erase(0, data[5].find(",")+1);
    }
    data[6].erase(data[6].find("size="), 5);
    size = atoi(data[6].c_str());
  }

  //save
  if(this->dataBlocks[addr] == 0)
  {
    this->dataBlocks[addr] = new Inode(isDir, UID, GID, this->logger);
  }
  Inode* temporary = dynamic_cast<Inode*>(this->dataBlocks[addr]);
  temporary->isDirectory = isDir;
  temporary->uid = UID;
  temporary->gid = GID;
  if(isDir)
  {
    temporary->directory.inodesInside = dirmap;
  }
  else
  {
    for(uint32_t i=0; i<sop::files::ConstEV::directAddrBlock; i++)
    {
      temporary->file.directBlockAddr[i] = direct[i];
    }
    temporary->file.indirectBlockAddr.clear();
    for(auto x : indirect)
    {
      temporary->file.indirectBlockAddr.push_back(x);
    }
    temporary->file.size = size;
  }
}

void sop::files::Filesystem::readData(uint32_t addr, std::string data)
{
  sop::files::Data* temp;
  if(this->dataBlocks[addr] == 0)
  {
    this->dataBlocks[addr] = new Data();
  }
  temp = dynamic_cast<sop::files::Data*>(this->dataBlocks[addr]);
  for(uint32_t i=0; i<sop::files::ConstEV::blockSize; i++)
  {
    temp->containter[i] = data[i];
  }
}

void sop::files::Filesystem::formatHandler(const std::vector<const std::string> & params)
{
  if(params.size() == 1)
  {
    sop::users::PermissionsManager pm;
    if(pm.isSuperUser(0))//permission check (root) ToDo current PID
    {
      this->format();
    }
    else
    {
      std::cout<<"No permission to format"<<std::endl;
    }
  }
  else
  {
    std::cout<<"format - reset disk to 0 state in format of EXT-like filesystem"<<std::endl;
  }
}

void sop::files::Filesystem::format()
{
  this->serialize->read();
  this->logger->logFiles(5, "Formating filesystem");
  this->logger->logFiles(4, "Setting free spaces and presetting structures");
  this->freeSpace.clear();
  for(uint32_t i=0; i < sop::files::ConstEV::numOfBlocks; i++)
  {
    this->freeSpace.push_back(i);
    if(this->dataBlocks[i] != 0)
    {
      delete this->dataBlocks[i];
    }
    this->dataBlocks[i] = 0;
  }
  std::sort(this->freeSpace.begin(), this->freeSpace.end());
  this->dataBlocks[0] = new sop::files::Inode(true, 0,0, this->logger);
  this->freeSpace.erase(this->freeSpace.begin());
  this->serialize->save();
  std::cout<<"Format successful"<<std::endl;
  this->logger->logFiles(6, "Filesystem format successful");
}

void sop::files::Filesystem::writeToFileP(sop::files::File* file_ptr, std::string input)
{
  file_ptr->writeToFile(input, &this->freeSpace);
}