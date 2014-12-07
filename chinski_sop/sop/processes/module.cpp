#include ".\sop\processes\module.h"
#include ".\sop\processes\class_process.h"

sop::processes::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel)
{

}

sop::processes::Module::~Module()
{

}

std::string sop::processes::Module::getClassName() const
{
  return "sop::processes::Module";
}

void sop::processes::Module::initialize()
{
}

//moje

void sop::processes::Module::addToVector(boost::shared_ptr<sop::process::Process>)
{
  sop::processes::Module::ProcessVector.push_back();
}

void sop::processes::Module::removeFromVector()
{
  sop::processes::Module::ProcessVector.pop_back();
}

void sop::processes::Module::showObjectInList(uint16_t PID)
{
  boost::shared_ptr<sop::process::Process> p = sop::processes::Module::findProcess(PID);
  if(p)
  {
    log(p->sop::process::Process::getPID());
  }
  
  else
  {
    // nie znaleziono procesu
  } 
}

boost::shared_ptr<sop::process::Process> sop::processes::Module::findProcess(uint16_t PID)
{
  boost::shared_ptr<sop::process::Process> process_found;
  std::vector<boost::shared_ptr<sop::process::Process>>::iterator it;
  for (it= ProcessVector.begin(); it!= ProcessVector.end(); ++it)
  {
    if ((*it)->sop::process::Process::getPID() == PID)
    {
      process_found = (*it);
      break;
    }
  }
  return process_found;
}