#ifndef _SOP_USERS_USER_H_
#define _SOP_USERS_USER_H_

#include <string>
#include <boost\shared_ptr.hpp>
#include ".\sop\users\id_definitions.h"

namespace sop
{
  namespace users
  {
    struct User{
      User();
      User(uid_t uid, gid_t gid, const std::string & username, const std::string & password, const std::string & info, const std::string & home_dir);
      uid_t uid;
      gid_t gid;
      std::string username;
      std::string password; //encrypted password
      std::string info;
      std::string home_dir;
    };

    extern boost::shared_ptr<User> nobody;
  }
}

#endif