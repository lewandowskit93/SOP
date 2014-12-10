#ifndef _SOP_USERS_FAKERS_H_
#define _SOP_USERS_FAKERS_H_

#include <boost\shared_ptr.hpp>
#include ".\sop\users\id_definitions.h"
#include ".\sop\users\permissions.h"

namespace sop
{
  namespace users
  {
    namespace fakers
    {
      struct pcb
      {
        pcb();
        pcb(uid_t uid, gid_t gid);
        uid_t uid;
        gid_t gid;
      };

      struct inode
      {
        inode();
        uid_t uid;
        gid_t gid;
        Permissions permissions;
      };

      extern boost::shared_ptr<pcb> shell_pcb;
      boost::shared_ptr<pcb> getProcess(uint16_t pid);

      extern inode someInode;
      inode* getFile(std::string filename);
    }
  }
}

#endif