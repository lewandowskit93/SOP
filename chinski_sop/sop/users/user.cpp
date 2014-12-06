#include ".\sop\users\user.h"

sop::users::User::User()
{
}

sop::users::User::User(uid_t uid, gid_t gid, const std::string & username, const std::string & password, const std::string & info, const std::string & home_dir)
{
  this->uid=uid;
  this->gid=gid;
  this->username=username;
  this->password=password;
  this->info=info;
  this->home_dir=home_dir;
}

boost::shared_ptr<sop::users::User> sop::users::nobody(new sop::users::User(sop::users::kMax_uid,sop::users::kMax_gid,"nobody","","User which holds max uid. Unloggable.","/home/nobody/"));