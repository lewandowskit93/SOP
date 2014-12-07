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

//deklaracje funkcji dzialjacych na wektorze

void sop::processes::Module::addToVector(boost::shared_ptr<sop::process::Process> object)
{
  sop::processes::Module::ProcessVector.push_back(object);
}

void sop::processes::Module::removeFromVector(uint16_t PID)
{
  boost::shared_ptr<sop::process::Process> p =  sop::processes::Module::findProcess(PID);
  sop::processes::Module::ProcessVector.erase(p);
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
    if ((*it)->getPID() == PID)
    {
      process_found = (*it);
      break;
    }
  }
  return process_found;
}

//stworzenie procesu INIT

void sop::processes::Module::CreateShellInit()
{
  boost::shared_ptr<sop::process::Process> Procesik (new sop::process::Process());
  Procesik->setPID(0);
  Procesik->setPPID(0);
  Procesik->setUID(0);
  Procesik->setGID(0);
  Procesik->setMemoryFlagStatus(0);
  Procesik->setProcessorFlagStatus(0);
  Procesik->setEndingFlagStatus(0);
  Procesik->setProcessIsInScheduler(0);
  Procesik->setIsActuallyRunning(1);
  sop::processes::Module::addToVector(Procesik);
}