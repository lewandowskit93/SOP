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
  sop::processes::Module::CreateShellInit(); //zainicjowanie stworzenia procesu INIT
  fillQueue(); //wypelnienie kolejki z PID'ami
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
    std::cout << "isRunning: " << ProcesToShow->getIsActuallyRunning() << std::endl;
    std::cout << "isTrue: " << ProcesToShow->getIsTrueProcess() << std::endl;
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
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Shell command executed: Process killed");
  }
  else
  {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Shell command executed: Init process cannot be killed");
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
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Shell command executed: Child process created");
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
  if (params.size()==3)
  {
    uint16_t PID = sop::StringConverter::convertStringTo<uint16_t>(params[params.size()-1]);
    boost::shared_ptr<sop::process::Process> ProcesForExec = sop::processes::Module::findProcess(PID);
    sop::processes::Module::exec(params[1], ProcesForExec);
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"shell command executed: file writted to memory");
  }
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
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"New process created and added to vector of processes");
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
  child->setIsTrueProcess(Parent->getIsTrueProcess());  //ustawienie na sztywno argumentu na 1 to zaslepka na wlaczenie drugiego forka
  child->setIsActuallyRunning(0); //ustawienie na 1 to zaslepka do sprawdzenia waita
  child->setMemoryFlagStatus(0);
  child->setProcessorFlagStatus(0);
  child->setProcessIsInScheduler(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"New child process created");
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS NEW");
  if (Parent->getIsTrueProcess() == 0)
  {    
    return;
  } 
  else if (Parent->getIsTrueProcess() == 1)
  {
    child->setArrayPages(/*allocate(Parent->getArrayPages(), child->getPID())*/200); //wartosc 200 to zaslepka symulujaca adres tablicy stronic
    if (/*child->getArrayPages().getPageTableSize()*/200 > 0) //wartosc 200 to zaslepka powodujaca ze pamiec zostanie zaalokowana, 0 zeby nie
    {
      child->setMemoryFlagStatus(1);
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory allocated");
    }
    else
    {
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::WARNING,"Process creation failed"); //zmodyfikowac zeby omijalo pozostale opcje
      return;      
    }
    //_kernel->getProcessorModule()->addProcess(child); //skomentowanie to zaslepka spowodowana brakiem funkcji. Przyjmuje sie ze proces zostal dodany do schedulera
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process added to scheduler");
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS: WAITING");
    if (child->getIsActuallyRunning() == 1)
    {
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS: RUNNING");
    }
    if (Parent->getIsActuallyRunning()==1 && child->getIsActuallyRunning()==1) //jesli paren jak i potomek flagi running ustawione na 1 to wywoluje wait
    {
      sop::processes::Module::wait(Parent, child); //wywolanie wait
      if (child->getEndingFlagStatus()==1 || child->getIsKilled()==1) //jesli potomek zostal wykonany standardowo albo zostal zabity ponownie alokuje pamiec dla parenta i wrzuca go do schedulera
      {
        Parent->setArrayPages(/*allocate(Parent->getArrayPages(), Parent->getPID())*/200); //zaslepka o wartosci 200 tak jak poprzednio
        if (/*Parent->getArrayPages().getPageTableSize()*/200 > 0) //zaslepka o wartosci 200 tka jak poprzednio
        {
          Parent->setMemoryFlagStatus(1);
          _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory allocated again to Parent process");
        }
        //_kernel->getProcessorModule()->addproces(Parent); // skomentowanie to zaslepka i przyjmujemy ze parent zostanie dodany do schedulera
        _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"parent process added to scheduler again");
      }
    }
  }  
}
//funkcja wpisujaca kod programu do wykonania
void sop::processes::Module::exec(std::string filename, boost::shared_ptr<sop::process::Process> Proces)
{
  //sop::files::File* filepointer = sop::files::Filesystem::openFile(Proces, getPathFromParam(filename), "x");
  //std::string data = _kernel->getFilesModule()->readFile(filepointer);
  //_kernel->getFilesModule()->closeFile(filepointer);
  //delete filepointer;
  if (/*Proces->getArrayPages().getPageTableSize()*Physical_drive.getFrameSize()*/200 < /*data.size()*/ 500) //porownuje czy rozmiar pliku jest mniejszy od zaalokowanej juz pamieci
  {
    //_kernel->getMemoryModule()->deallocate(Proces->getArrayPages()); //dealokacja juz przypisanej pamieci
    //_kernel->getMemoryModule()->allocate(data.size(), Proces->getPID()); //alokacja ponownie pamieci
    Proces->setArrayPages( /*_kernel->getMemoryModule()->allocate(data.size(), Proces->getPID())*/ 500); //zgranie z Fisza
    if (/*Proces->getArrayPages().getPageTableSize()*/ 500 > 0) //zgranie z Fisza
    {
      Proces->setMemoryFlagStatus(1);
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory allocated");
     // _kernel->getMemoryModule()->write(Proces->getArrayPages(),  data); //zapisanie kodu z pliku do pamieci
      Proces->setIsTrueProcess(1); //proces dostal kod wiec ustwiamy flage ze jest prawdziwy
    }
    else
    {
      Proces->setMemoryFlagStatus(0);
    }
  }
  else
  {
    //_kernel->getMemoryModule()->write(Proces->getArrayPages(), data); //zapisanie kodu z pliku do pamieci
    Proces->setIsTrueProcess(1); //proces dostal kod wiec ustwiamy flage ze jest prawdziwy
  }  
}
//funkcja wstrzymujaca proces macierzysty
void sop::processes::Module::wait(boost::shared_ptr<sop::process::Process> Parent, boost::shared_ptr<sop::process::Process> Child)
{
  Parent->setIsActuallyRunning(0);
  Parent->setProcessIsInScheduler(0);
  //_kernel->getProcessorModule()->removeProcess(Parent, Parent->getPID()); //skomentowanie to zaslepka sugerujaca ze proces zostal usuniety
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted from Scheduler");
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS: WAITING ON CHILD");
}
//funkcja zabijajaca proces o podanym PID
void sop::processes::Module::kill(boost::shared_ptr<sop::process::Process> Proces)
{
  boost::shared_ptr<sop::process::Process> p = Proces;
  //_kernel->getMemoryModule()->deallocate(p->getArrayPages()); //skomentowanie to zaslepka przyjmuje ze pamiec zostala zdealokowana
  p->setMemoryFlagStatus(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory deallocated");
  //_kernel->getProcessorModule()->removeProces(); // skomentowanie to zaslepka przyjmuje ze proces zostal usuniety ze schedulera
  p->setProcessorFlagStatus(0);
  p->setEndingFlagStatus(1);
  p->setIsKilled(1);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted from scheduler");
  sop::processes::Module::removeFromVector(p->getPID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted from vector of processes");
}
//funkcja zamykajaca proces
void sop::processes::Module::exit(boost::shared_ptr<sop::process::Process> Proces)
{
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PROCESS GET STATUS EXECUTED");
  boost::shared_ptr<sop::process::Process> p = Proces;
  //_kernel->getMemoryModule()->deallocate(p->getArrayPages()); // skomentowanie to zaslepka przyjmuje ze pamiec zostala zdealokowana
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory deallocated");
  Proces->setEndingFlagStatus(1);
  sop::processes::Module::removeFromVector(p->getPID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted from vector of processes");
  //Proces->setExitCode(/*??*/);
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
