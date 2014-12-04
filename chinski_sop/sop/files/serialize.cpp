#include "./sop/files/serialize.h"

#include <iostream>
#include <string>

sop::files::Serialize::Serialize(Filesystem* fsPtr, std::string filename) :
  filename(filename),
  filesystem_p(fsPtr)
{
}

sop::files::Serialize::~Serialize()
{
}

void sop::files::Serialize::save()
{
}

void sop::files::Serialize::read()
{
}