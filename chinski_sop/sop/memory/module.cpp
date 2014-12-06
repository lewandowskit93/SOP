#include ".\sop\memory\module.h"
#include <math.h>

sop::memory::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel),storage(4096,32)
{

}

sop::memory::Module::~Module()
{

}

std::string sop::memory::Module::getClassName() const
{
  return "sop::memory::Module";
}

void sop::memory::Module::initialize()
{
  
}
int8_t sop::memory::Module::calculatePages(int8_t program_size)
{
  float number_of_pages=(float)storage.getFrameSize();
  number_of_pages=ceil(number_of_pages/program_size);
  
  return (int8_t)number_of_pages;
}



