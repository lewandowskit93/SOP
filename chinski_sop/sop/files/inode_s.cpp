#include "./sop/files/inode_s.h"

#include <iostream>
#include <string>
#include <vector>
#include <boost/make_shared.hpp>

/*
dir=1
uid=123
gid=123
name  address
name  address

OR 

dir=0
uid=123
gid=123
direct=123,123,123,123,123,123
indirect=123,123,123,123,123
size=4
*/

sop::files::Inode_s::Inode_s(Inode* ptr, sop::logger::Logger* logger) :
  inode_p(ptr),
  logger(logger)
{
  this->logger->logFiles(3, "Inode serialization member");
}

sop::files::Inode_s::~Inode_s()
{
}

void sop::files::Inode_s::readDirectory(std::vector<std::string> partedData)
{
  this->logger->logFiles(3, "Reading directory");
  if(partedData.size() > 3)
  {
    for(uint32_t t=3; t<partedData.size(); t++)
    {
      this->dirMap[partedData[t].substr(0, partedData[t].find("\t"))] = atoi(partedData[t].substr(partedData[t].find(" "), partedData[t].size()-1).c_str());
    }
  }
}

std::string sop::files::Inode_s::writeDirectory()
{
  this->logger->logFiles(3, "Writing directory");
  std::string output = "";
  for(auto data : this->dirMap)
  {
    output += data.first + "\t" + std::to_string(data.second);
    if(data.first != this->dirMap.end()->first)
    {
      output += "\n";
    }
  }
  return output;
}

void sop::files::Inode_s::readFile(std::vector<std::string> partedData)
{
  this->logger->logFiles(3, "Reading file");
  uint32_t iterator = 0;
  if(partedData.size() > 3)
  {
    partedData[3] = partedData[3].substr(partedData[3].find("="), partedData[3].size()-1);
    while(partedData[3].size())
    {
      this->directBl[iterator] = atoi(partedData[3].substr(0, partedData[3].find(",")).c_str());
      iterator++;
      partedData[3] = partedData[3].substr(0, partedData[3].find(","));
    }
    partedData.erase(partedData.begin()+3);
  }
  if(partedData.size() > 3)
  {
    partedData[3] = partedData[3].substr(partedData[3].find("="), partedData[3].size()-1);
    while(partedData[3].size())
    {
      this->indirectBl.push_back(atoi(partedData[3].substr(0, partedData[3].find(",")).c_str()));
      partedData[3] = partedData[3].substr(0, partedData[3].find(","));
    }
  }
  this->size = atoi(partedData[3].substr(partedData[3].find("="), partedData[3].size()).c_str());
}

std::string sop::files::Inode_s::writeFile()
{
  this->logger->logFiles(3, "Writing file");
  std::string output = "direct=";
  for(uint32_t i=0; i<sop::files::ConstEV::directAddrBlock; i++)
  {
    output += this->directBl[i] + ",";
  }
  output += "\n";
  for(auto data : this->indirectBl)
  {
    output += data + ",";
  }
  output += "\nsize=" + std::to_string(this->size);
  return output;
}

void sop::files::Inode_s::readBlock(std::string data)
{
  this->logger->logFiles(3, "Reading data block");
  this->itemize(data);
  this->inode_p->isDirectory = this->isDirectory;
  this->inode_p->uid = this->uid;
  this->inode_p->gid = this->gid;
  this->assign();
}

std::string sop::files::Inode_s::writeBlock()
{
  this->logger->logFiles(3, "Writing data block");
  return this->serialize();
}

void sop::files::Inode_s::assign()
{
  this->logger->logFiles(3, "Assigning data");
  if(this->isDirectory)
  {
    for(std::map<std::string,uint32_t>::iterator iter=this->dirMap.begin(); iter != this->dirMap.end(); iter++)
    {
      this->inode_p->directory.inodesInside[iter->first] = iter->second;
    }
  }
  else
  {
    for(int i=0; i<sop::files::ConstEV::directAddrBlock; i++)
    {
      this->inode_p->file.directBlockAddr[i] = this->directBl[i];
    }
    this->inode_p->file.indirectBlockAddr.clear();
    for(uint32_t i=0; i<this->indirectBl.size(); i++)
    {
      this->inode_p->file.indirectBlockAddr.push_back(this->indirectBl[i]);
    }
    this->inode_p->file.size = this->size;
  }
}

void sop::files::Inode_s::itemize(std::string data)
{
  this->logger->logFiles(3, "Itemizing stream");
  if(data.find("dir") != std::string::npos && data.find("uid") != std::string::npos && data.find("gid") != std::string::npos)
  {
    std::vector<std::string> partedData;
    for(uint32_t t=0; t<data.size(); t++)
    {
      partedData.push_back(data.substr(0, data.find("\n")));
      data = data.substr(data.find("\n"), data.size()-1);
    }
    this->isDirectory = (0!=atoi(partedData[0].substr(4,partedData[0].size()-1).c_str()));
    this->uid = atoi(partedData[1].substr(4,partedData[1].size()-1).c_str());
    this->gid = atoi(partedData[2].substr(4,partedData[2].size()-1).c_str());
    if(this->isDirectory)
    {
      this->readDirectory(partedData); 
    }
    else
    {
      this->readFile(partedData);
    }
  }
  else
  {
    std::cout<<"Disk corrupted!!!"<<std::endl;
  }
}

std::string sop::files::Inode_s::serialize()
{
  this->logger->logFiles(3, "Serializing stream");
  std::string output;
  output +="dir=" + std::to_string(this->isDirectory) + "\n";
  output +="uid=" + std::to_string(this->uid) + "\n";
  output +="gid=" + std::to_string(this->gid) + "\n";
  if(this->isDirectory)
  {
    output += this->writeDirectory();
  }
  else
  {
    output += this->writeFile();
  }
  return output;
}