#include ".\sop\processes\module.h"
#include ".\sop\processes\class_process.h"
#include ".\sop\system\kernel.h"
#include ".\sop\logger\logger.h"

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

void sop::processes::Module::cH_showprocess(const std::vector<const std::string> & params)
{
  uint16_t PID;
  boost::shared_ptr<sop::process::Process> p = sop::processes::Module::findProcess(PID);
  if(p)
  {
    //log(p->getPID());
    std::cout << p->getPID() << std::endl;
    //.
    //.
    //.
  }
  
  else
  {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process not found");
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

void sop::processes::Module::cloneProcessTrue(boost::shared_ptr<sop::process::Process> Proces1, boost::shared_ptr<sop::process::Process> Proces2)
{

}

void sop::processes::Module::cloneProcessFalse(boost::shared_ptr<sop::process::Process> Proces1, boost::shared_ptr<sop::process::Process> Proces2)
{

}

void sop::processes::Module::createNewProcess()
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
  Procesik->setIsActuallyRunning(0);
  sop::processes::Module::addToVector(Procesik);
}

void sop::processes::Module::fork()
{
  sop::processes::Module::
  //if (isTrueProcess == 0)
  //stworz nowy proces
  //ustaw flage isrunning na 1
  //ustaw flagemomory na 0
  //ustaw flageprocesor na 0
  //ustaw flageisending na 0
  //pobierz nowy PID
  //przypisz PID procesu na ktorym wykonywany jest fork jako PPID
  //przypisz wartosci adresu tablicy stron, czyli 0

  //else if (isTrueProcess == 1)
  //stworz nowy proces
  //pobierz nowy PID
  //przypisz PID procesu na ktorym wykonywany jest fork jako PPID
  //skopiuj adres tablicy stron
  //skopiuj zawartosci rejestrow
  //ustaw flage is running na 0 co spowoduje poproszenie o procesor
}

void sop::processes::Module::exec()
{

}