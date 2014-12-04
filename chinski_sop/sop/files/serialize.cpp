#include "./sop/files/serialize.h"

#include <iostream>
#include <string>

sop::files::Serialize::Serialize(Filesystem* fsPtr, std::string filename, sop::logger::Logger* logger) :
  filename(filename),
  filesystem_p(fsPtr),
  logger(logger)
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