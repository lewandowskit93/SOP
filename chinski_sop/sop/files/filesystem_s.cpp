#include "./sop/files/filesystem_s.h"

#include <iostream>
#include <string>

sop::files::Filesystem_s::Filesystem_s(Filesystem* fsPtr) :
  filesystem_p(fsPtr)
{
}

sop::files::Filesystem_s::~Filesystem_s()
{
}

void sop::files::Filesystem_s::readData()
{
}

void sop::files::Filesystem_s::writeData()
{
  //this->filesystem_p->freeSpace
  //this->filesystem_p->dataBlocks
}