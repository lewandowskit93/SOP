#ifndef _SOP_USERS_ID_DEFINITIONS_H_
#define _SOP_USERS_ID_DEFINITIONS_H_

#include <stdint.h>

namespace sop
{
  namespace users
  {
    typedef uint16_t uid_t;
    typedef uid_t gid_t;
    typedef int8_t priority_t;
    extern const uid_t kMax_system_uid;
    extern const gid_t kMax_system_gid;
    extern const uid_t kMax_uid;
    extern const gid_t kMax_gid;
    extern const priority_t kMin_priority;
    extern const priority_t kDefault_priority;
    extern const priority_t kMax_priority;
  }
}


#endif