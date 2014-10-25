#include ".\sop\system\system.h"

sop::system::System::System():
  sop::Object(){
}

sop::system::System::~System(){
}

std::string sop::system::System::getClassName(){
  return "sop::system::System";
}

void sop::system::System::initializeSystem(){
}

void sop::system::System::runSystem(){
}

sop::logger::Logger * sop::system::System::getLogger(){
  return &_logger;
}

sop::system::Shell * sop::system::System::getShell(){
  return &_shell;
}