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
  sop::processes::Module::CreateShellInit();
  fillQueue();
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
  boost::shared_ptr<sop::process::Process> ProcesToShow = sop::processes::Module::findProcess(PID);
  if(ProcesToShow)
  {
    std::cout << "PID number: " << ProcesToShow->getPID() << std::endl;
    std::cout << "PPID number: " << ProcesToShow->getPPID() << std::endl;
    std::cout << "UID number: " << ProcesToShow->getUID() << std::endl;
    std::cout << "GID number: " << ProcesToShow->getGID() << std::endl;
    std::cout << "Array Table: " << ProcesToShow->getArrayPages() << std::endl;
    std::cout << "Register A: " << ProcesToShow->getRejestrA() << std::endl;
    std::cout << "Register B: " << ProcesToShow->getRejestrB() << std::endl;
    std::cout << "Register C: " << ProcesToShow->getRejestrC() << std::endl;
    std::cout << "Register D: " << ProcesToShow->getRejestrD() << std::endl <<std::endl;
  }  
  else
  {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process not found\n");
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
  boost::shared_ptr<sop::process::Process> ProcesToKill = sop::processes::Module::findProcess(PID);
  if (ProcesToKill->getPID() != 0)
  {
  sop::processes::Module::kill(ProcesToKill);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Shell command executed: Process killed\n");
  }
  else
  {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Shell command executed: Init process cannot be killed\n");
  }
}
//polecenie shellowskie tworzocy proces potomka
void sop::processes::Module::cH_fork(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()==1)
  {
    std::cout<<"fork [-h] PID"<<std::endl;
    std::cout<<"Creating child process"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  uint16_t PID = sop::StringConverter::convertStringTo<uint16_t>(params[params.size()-1]);
  boost::shared_ptr<sop::process::Process> ProcesToFork = sop::processes::Module::findProcess(PID);
  sop::processes::Module::fork(ProcesToFork);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Shell command executed: Child process created\n");
}
//polecenie wywolujace funkcje exec
void sop::processes::Module::cH_exec(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()==1)
  {
    std::cout<<"exec [-h] name_file PID"<<std::endl;
    std::cout<<"Run file and write code to memory"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  uint16_t PID = sop::StringConverter::convertStringTo<uint16_t>(params[params.size()-1]);
  boost::shared_ptr<sop::process::Process> ProcesForExec = sop::processes::Module::findProcess(PID);
  sop::processes::Module::exec(plik, ProcesForExec);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Shell command executed: File writted to memory\n");
}
//funkcja wstawiajaca proces do wektora procesow
void sop::processes::Module::addToVector(boost::shared_ptr<sop::process::Process> object)
{
  sop::processes::Module::ProcessVector.push_back(object);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process added to vector of processes\n");
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
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted\n");
  }
  else
  {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process not found so i don't know what to delete\n");
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
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process found\n");
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
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"INIT created and added to vector of processes\n");
}
//funkcja tworzaca nowy proces
boost::shared_ptr<sop::process::Process> sop::processes::Module::createNewProcess()
{
  boost::shared_ptr<sop::process::Process> Procesik (new sop::process::Process());
  Procesik->setPID(sop::processes::Module::getPIDfromList());
  Procesik->setPPID(0);
  Procesik->setUID(0);
  Procesik->setGID(0);
  Procesik->setMemoryFlagStatus(0);
  Procesik->setProcessorFlagStatus(0);
  Procesik->setEndingFlagStatus(0);
  Procesik->setProcessIsInScheduler(0);
  Procesik->setIsActuallyRunning(0);
  sop::processes::Module::addToVector(Procesik);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"New process created and added to vector of processes\n");
  return Procesik;
}
//funkcja tworzaca nowego potomka
void sop::processes::Module::fork(boost::shared_ptr<sop::process::Process> Parent)
{
  boost::shared_ptr<sop::process::Process> child;
  child = sop::processes::Module::createNewProcess();
  child->setPPID(Parent->getPID());
  child->setRejestrB(Parent->getRejestrB());
  child->setRejestrC(Parent->getRejestrC());
  child->setRejestrD(Parent->getRejestrD());
  child->setGID(Parent->getGID());
  child->setUID(Parent->getUID());
  child->setIsTrueProcess(Parent->getIsTrueProcess());
  child->setIsActuallyRunning(0);
  child->setMemoryFlagStatus(0);
  child->setProcessorFlagStatus(0);
  child->setProcessIsInScheduler(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"New child process created\n");
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS NEW\n");
  if (Parent->getIsTrueProcess() == 0)
  {    
    return;
  } 
  else if (Parent->getIsTrueProcess() == 1)
  {
    child->setArrayPages(allocate(rozmiar, child->getPID())); //zgranie z Fisza
    if (child->getArrayPages().getPageTableSize() > 0) //zgranie z Fisza
    {
      child->setMemoryFlagStatus(1);
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory allocated");
    }
    sop::processor::Module::addProcess(child); //zgranie z Krzysio
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process added to scheduler");
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS: WAITING");
    if (child->getIsActuallyRunning() == 1)
    {
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS: RUNNING");
    }
  }
  
}
//funkcja wpisujaca kod programu do wykonania
void sop::processes::Module::exec(std::string filename, boost::shared_ptr<sop::process::Process> Proces)
{
  sop::files:File* filepointer = sop::files::Filesystem::openFile(Proces, getPathFromParam(filename), "x");
  std::string data = _kernel->getFilesModule()->readFile(filepointer);
  _kernel->getFilesModule()->closeFile(filepointer);
  delete filepointer;
  if (Proces->getArrayPages().getPageTableSize()*Physical_drive.getFrameSize() < data.size()) //porownuje czy rozmiar pliku jest mniejszy od zaalokowanej juz pamieci
  {
    _kernel->getMemoryModule()->deallocate(Proces->getArrayPages()); //dealokacja juz przypisanej pamieci
    _kernel->getMemoryModule()->allocate(data.size(), Proces->getPID); //alokacja ponownie pamieci
    Proces->setArrayPages( _kernel->getMemoryModule()->allocate(data.size(), Proces->getPID())); //zgranie z Fisza
    if (child->getArrayPages().getPageTableSize() > 0) //zgranie z Fisza
    {
      child->setMemoryFlagStatus(1);
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory allocated");
      _kernel->getMemoryModule()->write(Proces->getArrayPages,  data); //zapisanie kodu z pliku do pamieci
    }
    else
    {
      child->setMemoryFlagStatus(0);
    }
  }
  else
  {
    sop::memory::Module::write(Proces->getArrayPages, sop::file::module::perFile()); //zapisanie kodu z pliku do pamieci
  }
    
    
  
}
//funkcja wstrzymujaca proces macierzysty
void sop::processes::Module::wait(boost::shared_ptr<sop::process::Process> Parent, boost::shared_ptr<sop::process::Process> Child)
{
  Parent->setIsActuallyRunning(0);
  Parent->setProcessIsInScheduler(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS: WAITING ON CHILD\n");
}
//funkcja zabijajaca proces o podanym PID
void sop::processes::Module::kill(boost::shared_ptr<sop::process::Process> Proces)
{
  boost::shared_ptr<sop::process::Process> p = Proces;
  sop::memory::Module::deallocate(rozmiar, p->getPID()); //zgranie z Fisza  
  p->setMemoryFlagStatus(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory deallocated\n");
  sop::processor::Module::removeProces(); //zgranie z Krzysio
  p->setProcessorFlagStatus(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted from scheduler\n");
  sop::processes::Module::removeFromVector(p->getPID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted from vector of processes\n");
}
//funkcja zamykajaca proces
void sop::processes::Module::exit(boost::shared_ptr<sop::process::Process> Proces)
{
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS EXECUTED\n");
  boost::shared_ptr<sop::process::Process> p = Proces;
  sop::memory::Module::deallocate(rozmiar, p->getPID); //zgranie z Fisza
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory deallocated\n");
  sop::processes::Module::removeFromVector(p->getPID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted from vector of processes\n");
}
//funkcja wypelniajaca kolejke pidow
void sop::processes::Module::fillQueue()
 {
   for (uint16_t i = 1; i<=100; i++)
   {
     sop::processes::Module::PIDlist.push(i);
   }
 }
//funkcja pobierajaca PID z listy
uint16_t sop::processes::Module::getPIDfromList()
{
  uint16_t PIDnumber = PIDlist.front();
  PIDlist.pop();
  return PIDnumber;
}
