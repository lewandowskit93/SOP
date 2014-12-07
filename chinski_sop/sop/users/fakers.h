#ifndef _SOP_USERS_FAKERS_H_
#define _SOP_USERS_FAKERS_H_

#include ".\sop\users\id_definitions.h"
#include ".\sop\users\permissions.h"

namespace sop
{
  namespace users
  {
    struct pcb
    {
        uid_t uid;
        gid_t gid;
    };

    struct inode
    {
        uid_t uid;
        gid_t gid;
        Permissions permissions;
    };
  }
}

#endif