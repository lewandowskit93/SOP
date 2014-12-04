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

void sop::process::Process::setRegisterA(uint16_t A)
{
  this->_register_A = A;
}

void sop::process::Process::setRegisterB(uint16_t B)
{
  this->_register_B = B;
}

void sop::process::Process::setRegisterC(uint16_t C)
{
  this->_register_C = C;
}

void sop::process::Process::setRegisterD(uint16_t D)
{
  this->_register_D = D;
}

void sop::process::Process::setStackCounter(uint16_t stack)
{
  this->_stack_counter = stack;
}

void sop::process::Process::setArrayPages(uint16_t ArrayPages)
{
  this->_array_pages = ArrayPages;
}

    //getters



//private

    //setters
void sop::process::Process::setPPID(uint16_t PPID)
{
  this->_PPID = PPID;
}

    //getters
uint16_t sop::process::Process::getPPID()
{
  return this->_PPID;
}