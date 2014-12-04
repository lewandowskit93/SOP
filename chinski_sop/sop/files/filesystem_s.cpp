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
<block id=1 type=file>
32 bytes
<block id=2 type=inode>
...
*/

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