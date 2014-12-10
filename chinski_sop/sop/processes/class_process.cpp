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
    _processIsInScheduler(0),
    _isActuallyRunning(0),
    _isTrueProcess(0),
    _isKilled(0),
    _rejestrA(0),
    _rejestrB(0),
    _rejestrC(0),
    _rejestrD(0)
{}
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

uint16_t sop::process::Process::getRejestrA()
{
  return this->_rejestrA;
}

uint16_t sop::process::Process::getRejestrB()
{
  return this->_rejestrB;
}

uint16_t sop::process::Process::getRejestrC()
{
  return this->_rejestrC;
}

uint16_t sop::process::Process::getRejestrD()
{
  return this->_rejestrD;
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

uint16_t sop::process::Process::getIsTrueProcess()
{
  return this->_isTrueProcess;
}

uint16_t sop::process::Process::getIsKilled()
{
  return this->_isKilled;
}

uint8_t sop::process::Process::setExitCode()
{
  return this->_exitCode;
}

//setters from protected
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

void sop::process::Process::setRejestrA(uint16_t A)
{
  this->_rejestrA = A;
}

void sop::process::Process::setRejestrB(uint16_t B)
{
  this->_rejestrB = B;
}

void sop::process::Process::setRejestrC(uint16_t C)
{
  this->_rejestrC = C;
}

void sop::process::Process::setRejestrD(uint16_t D)
{
  this->_rejestrD = D;
}

//set enums from private
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

    //setters from private
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

void sop::process::Process::setIsTrueProcess(uint16_t ITP)
{
  this->_isTrueProcess = ITP;
}

void sop::process::Process::setIsKilled(uint16_t IK)
{
  this->_isKilled = IK;
}

void sop::process::Process::setExitCode(uint8_t EC)
{
  this->_exitCode = EC;
}

    


