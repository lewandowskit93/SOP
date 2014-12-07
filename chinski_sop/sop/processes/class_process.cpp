#include "./sop/processes/module.h"
#include "./sop/processes/class_process.h"

//public
    //construtor
sop::process::Process::Process():
    _PID(0),
    _UID(0),
    _GID(0),
    _array_pages(0),
    _PPID(0),
    _memoryFlagStatus(0),
    _processorFlagStatus(0),
    _endingFlagStatus(0),
    _processIsInScheduler(0){}
    //destructor
sop::process::Process::~Process()
{
}
    //getters from protected
uint16_t sop::process::Process::getPID()
{
  return this->_PID;
}

uint16_t sop::process::Process::getUID()
{
  return this->_UID;
}

uint16_t sop::process::Process::getGID()
{
  return this->_GID;
}

uint16_t sop::process::Process::getArrayPages()
{
  return this->_array_pages;
}
    //getters from private
uint16_t sop::process::Process::getPPID()
{
  return this->_PPID;
}

uint16_t sop::process::Process::getMemoryFlagStatus()
{
  return this->_memoryFlagStatus;
}

uint16_t sop::process::Process::getProcessorFlagStatus()
{
  return this->_processorFlagStatus;
}

uint16_t sop::process::Process::getEndingFlagStatus()
{
  return this->_endingFlagStatus;
}

uint16_t sop::process::Process::getProcessIsInScheduler()
{
  return this->_processIsInScheduler;
}

uint16_t sop::process::Process::getIsActuallyRunning()
{
  return this->_isActuallyRunning;
}
//protected
    //setters
void sop::process::Process::setPID(uint16_t PID)
{
  this->_PID = PID;
}

void sop::process::Process::setUID(uint16_t UID)
{
  this->_UID = UID;
}

void sop::process::Process::setGID(uint16_t GID)
{
  this->_GID = GID;
}

void sop::process::Process::setArrayPages(uint16_t ArrayPages)
{
  this->_array_pages = ArrayPages;
}
//private
    //set enums
uint16_t sop::process::Process::setStatusNew() //ustawia status na nowy
{
  TASK_STATUS status;
  status = task_new;
  return status;
}

uint16_t sop::process::Process::setStatusWaiting() //ustawia status na oczekujacy
{
  TASK_STATUS status;
  status = task_waiting;
  return status;
}

uint16_t sop::process::Process::setStatusRunning() //ustawia status na uruchomiony
{
  TASK_STATUS status;
  status = task_running;
  return status;
}

uint16_t sop::process::Process::setStatusExecuted() //ustawia status na wykonany
{
  TASK_STATUS status;
  status = task_executed;
  return status;
}
    //setters  
void sop::process::Process::setPPID(uint16_t PPID)
{
  this->_PPID = PPID;
}

void sop::process::Process::setMemoryFlagStatus(uint16_t MFS)
{
  this->_memoryFlagStatus = MFS;
}

void sop::process::Process::setProcessorFlagStatus(uint16_t PFS)
{
  this->_processorFlagStatus = PFS;
}

void sop::process::Process::setEndingFlagStatus(uint16_t EFS)
{
  this->_endingFlagStatus = EFS;
}

void sop::process::Process::setProcessIsInScheduler(uint16_t PIIS)
{
  this->_processIsInScheduler = PIIS;
}

void sop::process::Process::setIsActuallyRunning(uint16_t IAR)
{
  this->_isActuallyRunning = IAR;
}

    


