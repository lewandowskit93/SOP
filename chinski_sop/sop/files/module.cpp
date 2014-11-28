#include ".\sop\files\module.h"
#include ".\sop\system\kernel.h"
#include ".\sop\files\filesystem.h"

sop::files::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel)
{

}

sop::files::Module::~Module()
{

}

std::string sop::files::Module::getClassName() const
{
  return "sop::files::Module";
}

void sop::files::Module::initialize()
{
  Filesystem fs;
  sop::system::Shell* shell = this->_kernel->getShell();
  sop::logger::Logger* logger = this->_kernel->getLogger();
  shell->registerCommand("cd",&Filesystem::changeDirectoryHandler,&fs);
  //shell->registerCommand("mv",&Filesystem::moveHandler,&fs);
  //shell->registerCommand("rm",&Filesystem::removeFileHandler,&fs);
  //shell->registerCommand("nano",&Filesystem::nanoHandler,&fs);
  //shell->registerCommand("touch",&Filesystem::createFileHandler,&fs);
  //shell->registerCommand("mkdir",&Filesystem::createDirectoryHandler,&fs);
  //shell->registerCommand("rmdir",&Filesystem::removeDirectoryHandler,&fs);
  //shell->registerCommand("find",&Filesystem::seekHandler,&fs);
}