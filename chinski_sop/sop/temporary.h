#ifndef _sakdfasdhfkasjfla_
#define _sakdfasdhfkasjfla_

#include <cstdint>
#include <boost\shared_ptr.hpp>
#include "./sop/object.h"
#include ".\sop\users\id_definitions.h"

namespace sop
{

  namespace process
  {
    class Process
    {
    public:
        sop::users::uid_t uid;
        sop::users::gid_t gid;
    protected:
    private:
    };
    extern boost::shared_ptr<sop::process::Process> shell_pcb;
    boost::shared_ptr<sop::process::Process> getProcess(uint16_t pid);
  };

};
#endif