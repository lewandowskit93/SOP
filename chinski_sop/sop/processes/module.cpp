#include ".\sop\processes\module.h"
#include ".\sop\processes\class_process.h"
#include ".\sop\system\kernel.h"
#include ".\sop\logger\logger.h"
#include ".\sop\string_converter.h"

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

//rejestrowanie komend shellowskich
void sop::processes::Module::initialize()
{
  _kernel->getShell()->registerCommand("showprocess",&sop::processes::Module::cH_showprocess,this); //zarejestrowanie komendy showprocess
  _kernel->getShell()->registerCommand("kill",&sop::processes::Module::cH_kill,this); //zarejestrowanie komendy kill
  _kernel->getShell()->registerCommand("fork",&sop::processes::Module::cH_fork,this); //zarejestrowanie komendy fork
  _kernel->getShell()->registerCommand("exec",&sop::processes::Module::cH_exec,this); //zarejestrowanie komendy exec
}
//polecenie shelllowskie wypisujace na ekran informacje o procesie
void sop::processes::Module::cH_showprocess(const std::vector<const std::string> & params)
{  
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()==1)
  {
    std::cout<<"showprocess [-h] PID"<<std::endl;
    std::cout<<"Displays informations about process"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }

  uint16_t PID = sop::StringConverter::convertStringTo<uint16_t>(params[params.size()-1]);
  boost::shared_ptr<sop::process::Process> p = sop::processes::Module::findProcess(PID);
  if(p)
  {
    std::cout << "PID number: " << p->getPID() << std::endl;
    std::cout << "UID number: " << p->getUID() << std::endl;
    std::cout << "Array Table: " << p->getArrayPages << std::endl;
    std::cout << "Register A: " << p->getRejestrA << std::endl;
    std::cout << "Register B: " << p->getRejestrB << std::endl;
    std::cout << "Register C: " << p->getRejestrC << std::endl;
    std::cout << "Register D: " << p->getRejestrD << std::endl;
  }  
  else
  {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process not found");
  } 
}
//polecenie shellowskie zabijajace proces o podanym PID
void sop::processes::Module::cH_kill(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()==1)
  {
    std::cout<<"kill [-h] PID"<<std::endl;
    std::cout<<"Killing process with the specified PID number"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  uint16_t PID = sop::StringConverter::convertStringTo<uint16_t>(params[params.size()-1]);
  sop::processes::Module::kill(PID);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process killed");
}
//polecenie shellowskie tworzocy proces potomka
void sop::processes::Module::cH_fork(const std::vector<const std::string> & params)
{

}
//polecenie wywolujace funkcje exec
void sop::processes::Module::cH_exec(const std::vector<const std::string> & params)
{

}
//funkcja wstawiajaca proces do wektora procesow
void sop::processes::Module::addToVector(boost::shared_ptr<sop::process::Process> object)
{
  sop::processes::Module::ProcessVector.push_back(object);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process added to vector of processes");
}
//funkcja usuwajaca proces z wektora procesow
void sop::processes::Module::removeFromVector(uint16_t PID)
{
  std::vector<boost::shared_ptr<sop::process::Process>>::iterator it=ProcessVector.begin();
  for(;it!=ProcessVector.end();++it)
  {
    if((*it)->getPID() == PID) break;
  }
  if(it!=ProcessVector.end())
  {
    ProcessVector.erase(it);
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted");
  }
  else
  {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process not found so i don't know what to delete");
  }
}
//funkcja wyszukujaca proces o podanym PID
boost::shared_ptr<sop::process::Process> sop::processes::Module::findProcess(uint16_t PID)
{
  boost::shared_ptr<sop::process::Process> process_found;
  std::vector<boost::shared_ptr<sop::process::Process>>::iterator it;
  for (it= ProcessVector.begin(); it!= ProcessVector.end(); ++it)
  {
    if ((*it)->getPID() == PID)
    {
      process_found = (*it);
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process found");
      break;
    }
  }
  return process_found;
}
//funkcja tworzaca proces init/shell
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
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"INIT created and added to vector of processes");
}
//funkcja tworzaca nowy proces
boost::shared_ptr<sop::process::Process> sop::processes::Module::createNewProcess()
{
  boost::shared_ptr<sop::process::Process> Procesik (new sop::process::Process());
  Procesik->setPID(sop::processes::Module::getNewPID()); //napisaæ funkcje);
  Procesik->setPPID(0);
  Procesik->setUID(0);
  Procesik->setGID(0);
  Procesik->setMemoryFlagStatus(0);
  Procesik->setProcessorFlagStatus(0);
  Procesik->setEndingFlagStatus(0);
  Procesik->setProcessIsInScheduler(0);
  Procesik->setIsActuallyRunning(0);
  sop::processes::Module::addToVector(Procesik);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"New process created and added to vector of processes");
  return Procesik;
}
//funkcja tworzaca nowego potomka
void sop::processes::Module::fork(boost::shared_ptr<sop::process::Process> Parent)
{
  boost::shared_ptr<sop::process::Process> p;
  p = sop::processes::Module::createNewProcess();
  p->setRejestrA(0);
  Parent->setRejestrA(p->getPID());
  p->setRejestrB(Parent->getRejestrB());
  p->setRejestrC(Parent->getRejestrC());
  p->setRejestrD(Parent->getRejestrD());
  p->setGID(Parent->getGID());
  p->setUID(Parent->getUID());
  p->setIsTrueProcess(Parent->getIsTrueProcess());
  p->setIsActuallyRunning(0);
  p->setMemoryFlagStatus(0);
  p->setProcessorFlagStatus(0);
  p->setProcessIsInScheduler(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"New child process created");
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS NEW");
  if (Parent->getIsTrueProcess() == 0)
  {    
    return;
  } 
  else if (Parent->getIsTrueProcess() == 1)
  {
    p->setArrayPages(allocate(rozmiar, p->getPID())); //zgranie z Fisza
    if (p->getArrayPages().getPageTableSize() > 0) //zgranie z Fisza
    {
      p->setMemoryFlagStatus(1);
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory allocated");
    }
    addProcess(p); //zgranie z Krzysio
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process added to scheduler");
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS WAITING");
  }
  
}
//funkcja
void sop::processes::Module::exec()
{

}
//funkcja przypisujaca nowy PID
uint16_t sop::processes::Module::getNewPID()
{

}
//funkcja wstrzymujaca proces macierzysty
void sop::processes::Module::wait(boost::shared_ptr<sop::process::Process> Parent, boost::shared_ptr<sop::process::Process> Child)
{

}
//funkcja zabijajaca proces o podanym PID
void sop::processes::Module::kill(boost::shared_ptr<sop::process::Process> Proces)
{
  boost::shared_ptr<sop::process::Process> p = Proces;
  sop::memory::Module::deallocate(rozmiar, p->getPID()); //zgranie z Fisza  
  p->setMemoryFlagStatus(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory deallocated");
  sop::processor::Module::removeProces(); //zgranie z Krzysio
  p->setProcessorFlagStatus(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted from scheduler");
  sop::processes::Module::removeFromVector(p->getPID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted from vector of processes");
}
//funkcja zamykajaca proces
void sop::processes::Module::exit(boost::shared_ptr<sop::process::Process> Proces)
{
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS EXECUTED");
  boost::shared_ptr<sop::process::Process> p = Proces;
  sop::memory::Module::deallocate(rozmiar, p->getPID); //zgranie z Fisza
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory deallocated");
  sop::processes::Module::removeFromVector(p->getPID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted from vector of processes");
}
