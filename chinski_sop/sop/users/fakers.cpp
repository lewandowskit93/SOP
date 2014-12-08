#include ".\sop\users\fakers.h"

boost::shared_ptr<sop::users::fakers::pcb> sop::users::fakers::shell_pcb 
  = boost::shared_ptr<sop::users::fakers::pcb>(new sop::users::fakers::pcb());

sop::users::fakers::pcb::pcb():
  uid(0),
  gid(0)
{
}

sop::users::fakers::pcb::pcb(uid_t uid, gid_t gid):
  uid(uid),
  gid(gid)
{
}

boost::shared_ptr<sop::users::fakers::pcb> sop::users::fakers::getProcess(uint16_t pid)
{
  return shell_pcb;
}