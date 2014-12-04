#include "./sop/files/filesystem_s.h"

#include <iostream>
#include <string>

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

sop::files::Filesystem_s::Filesystem_s(Filesystem* fsPtr, sop::logger::Logger* logger) :
  filesystem_p(fsPtr),
  logger(logger)
{
  this->logger->logFiles(3, "Filesystem serialization member");
}

sop::files::Filesystem_s::~Filesystem_s()
{
}

void sop::files::Filesystem_s::readData()
{
  this->logger->logFiles(7, "Reading data from disk");
}

void sop::files::Filesystem_s::writeData()
{
  this->logger->logFiles(7, "Writing data to disk");
  //this->filesystem_p->freeSpace
  //this->filesystem_p->dataBlocks
}