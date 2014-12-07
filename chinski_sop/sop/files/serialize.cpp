#include "./sop/files/serialize.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include "./sop/files/filesystem_s.h"

/*
<HARDDISK V0.7>
freesp=1,2,3,4,5
#$#
<block id=0 type=inode>
dir=1
uid=123
gid=123
string [tab] blockID
<block id=1 type=file size=x-bytes>
x bytes
<block id=2 type=inode>
...
*/

sop::files::Serialize::Serialize(Filesystem* fsPtr, std::string filename, sop::logger::Logger* logger) :
  filename(filename),
  filesystem_p(fsPtr),
  logger(logger)
{
  this->serialFilesystem = new sop::files::Filesystem_s(this->filesystem_p, this->logger);
}

sop::files::Serialize::~Serialize()
{
  delete this->serialFilesystem;
}

void sop::files::Serialize::save()
{
  //ToDo
  std::fstream fs;
  fs.open(this->filename, std::fstream::out);
  fs<<"<HARDDISK V0.7>\n";
  this->serialize(fs);
  fs.close();
}

void sop::files::Serialize::read()
{
  this->logger->logFiles(6, "Attempting to read hard disk");
  std::fstream fs;
  fs.open(this->filename, std::fstream::in);
  std::string tmp;
  if(!fs.good())
  {
    this->logger->logFiles(2, "Hard disk corrupted");
    std::cout<<"Hard disk corrupted!"<<std::endl;
    return;
  }
  std::getline(fs,tmp);
  //std::cout<<tmp<<std::endl;
  if(tmp == "<HARDDISK V0.7>")
  {
    this->itemize(fs);
  }
  else
  {
    this->logger->logFiles(2, "Hard disk corrupted");
    std::cout<<"Hard disk corrupted!"<<std::endl;
    return;
  }
}

void sop::files::Serialize::serialize(std::fstream& fs)
{
  //serialize data from filesystem_s
  std::vector<std::string> out = this->serialFilesystem->writeData();
  for(uint32_t i=0; i<out.size(); i++)
  {
    fs<<out[i]<<std::endl;
  }
}
 
void sop::files::Serialize::itemize(std::fstream& fs)
{
  //transfer items to filesystem_s
  std::vector<std::string> output;
  while(!fs.eof())
  {
    std::string tmp;
    std::getline(fs,tmp);
    output.push_back(tmp);
    //std::cout<<tmp<<std::endl;
  }
  this->serialFilesystem->readData(output);
}