#include "./sop/files/inode_s.h"

#include <iostream>
#include <string>
#include <vector>

sop::files::Inode_s::Inode_s(Inode* ptr) :
  inode_p(ptr)
{
}

sop::files::Inode_s::~Inode_s()
{
}

std::vector<char> sop::files::Inode_s::readDirectory()
{
  return *new std::vector<char>;
}

void sop::files::Inode_s::writeDirectory()
{
  //this->inode_p->directory
}

std::vector<char> sop::files::Inode_s::readFile()
{
  return *new std::vector<char>;
}

void sop::files::Inode_s::writeFile()
{
  //this->inode_p->file
}