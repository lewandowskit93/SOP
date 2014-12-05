#include "./sop/processes/class_process.h"

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

    //getters
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

//private
void sop::process::Process::sendToScheduler()
{

}

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

void sop::process::Process::setMemoryFlagStatus(uint16_t MF)
{
  this->_memoryFlagStatus = MF;
}

void sop::process::Process::setProcessorFlagStatus(uint16_t PF)
{
  this->_processorFlagStatus = PF;
}

    //getters
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

    //methods
