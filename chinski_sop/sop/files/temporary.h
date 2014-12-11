#ifndef _sakdfasdhfkasjfla_
#define _sakdfasdhfkasjfla_

#include <cstdint>
#include <boost\shared_ptr.hpp>
#include "./sop/object.h"

namespace sop
{
  namespace files
  {
    class Inode;
  };

  namespace process
  {
    class Process
    {
    public:
    protected:
    private:
    };
  };

  namespace users
  {
    typedef uint8_t permission_t;

    class PermissionsManager : public Object
    {
    public:
      bool hasPermission(sop::files::Inode* node, boost::shared_ptr<sop::process::Process> process, sop::users::permission_t mode){ return true; }
      bool isSuperUser(boost::shared_ptr<sop::process::Process> process){ return true; }
    };

    struct Permissions
    {
      static const permission_t kRWX=7;
      static const permission_t kRW=6;
      static const permission_t kRX=5;
      static const permission_t kR=4;
      static const permission_t kWX=3;
      static const permission_t kW=2;
      static const permission_t kX=1;
      static const permission_t kNone=0;

      Permissions() : user(kNone), group(kNone), others(kNone){}
      Permissions(bool for_directory) : user(kNone), group(kNone), others(kNone){}
      Permissions(permission_t user, permission_t group, permission_t others) : user(user), group(group), others(others){}

      permission_t user;
      permission_t group;
      permission_t others;
    };

    class PermissionsUtilities
    {
    public:
      static std::string getRWXString(Permissions permissions){ return "---------"; }
    };
  };
};
#endif