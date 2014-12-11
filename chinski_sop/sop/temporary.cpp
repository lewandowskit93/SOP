#include "temporary.h"
boost::shared_ptr<sop::process::Process> sop::process::shell_pcb;
boost::shared_ptr<sop::process::Process> sop::process::getProcess(uint16_t pid)
{
  return shell_pcb;
}