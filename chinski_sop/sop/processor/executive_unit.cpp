#include "./sop/processor/executive_unit.h"
#include "./sop/processor/scheduler.h"
#include "./sop/processor/interpreter.h"
#include "./sop/system/shell.h"
#include <iostream>
sop::processor::ExecutiveUnit::ExecutiveUnit(sop::logger::Logger* logger):
  logger(logger),
  _standardQuantTime(1)
  {
    this->logger->logProcessor(3,"Initializing processing unit.");
    this->logger->logProcessor(3,"Looking for errors...");
    resetQuantTime();
    this->logger->logProcessor(3,"Initialization done.");
    testerMethod();
  }

void sop::processor::ExecutiveUnit::testerMethod()
{
  sop::process::Process *test = new sop::process::Process();
  test->procek.a = 65535;
  scheduler.addToActiveTaskArray(test);
}

sop::processor::ExecutiveUnit::~ExecutiveUnit(){
}
void sop::processor::ExecutiveUnit::activateProcessor()
{
  _runningProcess = scheduler.getHighestPriorityProcess();
  _lastUsedProcess = _runningProcess;
}
short sop::processor::ExecutiveUnit::getQuantTimeLeft()
{
  return _quantTimeLeft;
}
void sop::processor::ExecutiveUnit::resetQuantTime()
{
  _quantTimeLeft = _standardQuantTime;
}
std::string sop::processor::ExecutiveUnit::processorTick()
{
  interpreter.buildProgramLine(_runningProcess); 
  std::string msg = interpreter.interpretLine(_runningProcess);
  interpreter.interpreterReset();
  return msg;
}

void sop::processor::ExecutiveUnit::mainExecutiveLoop()
{
  activateProcessor();
  int i = getQuantTimeLeft();
  while (i!=0)
  {
    if (processorTick()=="EXT")
    {
      return;
    }
    i--;
  }
  scheduler.addToUnactiveTaskArray(_runningProcess);
  _runningProcess = nullptr; //disactivating processor
  if (scheduler.isEraChangeNeeded())
    scheduler.eraChange();
}


void sop::processor::ExecutiveUnit::cH_showQuantTimeLeft(const std::vector<const std::string> & params)
{
  if (!sop::system::Shell::hasParam(params,"-h"))
  {
    this->logger->logProcessor(3, "Quant time left:");
    short q = getQuantTimeLeft();
    std::cout<<q<<std::endl;
  }
  else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"sqtleft [-h]"<<std::endl;
    std::cout<<"Shows the quant time that is left"<<std::endl;
  }
}

void sop::processor::ExecutiveUnit::cH_showActiveTaskQueue(const std::vector<const std::string> & params)
{
  if (!sop::system::Shell::hasParam(params,"-h"))
  {
    this->logger->logProcessor(3, "Active task list:");
    scheduler.printOutActiveTasks();
  }
  else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"sacttask [-h]"<<std::endl;
    std::cout<<"Shows the queue of active tasks"<<std::endl;
  }
}

void sop::processor::ExecutiveUnit::cH_showUnactiveTaskQueue(const std::vector<const std::string> & params)
{
  if (!sop::system::Shell::hasParam(params,"-h"))
  {
    this->logger->logProcessor(3, "Unactive task list:");
    scheduler.printOutUnactiveTasks();
  }
  else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"suacttask [-h]"<<std::endl;
    std::cout<<"Shows the queue of unactive tasks"<<std::endl;
  }
}

void sop::processor::ExecutiveUnit::cH_showActualProcessorState(const std::vector<const std::string> & params)
{
   if (!sop::system::Shell::hasParam(params,"-h"))
   {
     ProcessorHandler::printOutProcessorState(&_lastUsedProcess->procek);
   }
   else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
   {
    std::cout<<"procstate [-h]"<<std::endl;
    std::cout<<"Shows the state of the processor and its fields"<<std::endl;
   }
}

void sop::processor::ExecutiveUnit::cH_showAnyProcessorState(const std::vector<const std::string> & params)
{
   if (1)
   {
    //to be done eventually

   }
   else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
   {
    std::cout<<"procstate [-h] [-PID]"<<std::endl;
    std::cout<<"Shows the state of the processor(PID) and its fields"<<std::endl;
   }
}

void sop::processor::ExecutiveUnit::cH_fullTick(const std::vector<const std::string> & params)
{
  //testerMethod();
  if (!sop::system::Shell::hasParam(params,"-h"))
   {
     mainExecutiveLoop();
   }
   else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
   {
    std::cout<<"fulltick [-h]"<<std::endl;
    std::cout<<"Takes task with highest priority, and gives it processor resourcers, func. stops if quant time is 0 or finds EXT executed code "<<std::endl;
   }
}
