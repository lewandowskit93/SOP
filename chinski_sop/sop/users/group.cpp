#include ".\sop\users\group.h"


sop::users::Group::Group()
{
}

sop::users::Group::Group(gid_t gid, const std::string & group_name)
{
  this->gid=gid;
  this->group_name=group_name;
}

boost::shared_ptr<sop::users::Group> sop::users::nogroup(new sop::users::Group(sop::users::kMax_gid,"nogroup"));