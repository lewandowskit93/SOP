#ifndef _SOP_USERS_GROUP_H_
#define _SOP_USERS_GROUP_H_

#include <string>
#include <list>
#include <boost\shared_ptr.hpp>
#include ".\sop\users\id_definitions.h"

namespace sop
{
  namespace users
  {
    struct User;

    struct Group
    {
      Group();
      Group(gid_t gid, const std::string & group_name);
      gid_t gid;
      std::string group_name;
      std::list<boost::shared_ptr<User>> users_list;
    };

    extern boost::shared_ptr<Group> nogroup;
  }
}

#endif