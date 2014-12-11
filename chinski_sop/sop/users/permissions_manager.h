#ifndef _SOP_USERS_PERMISSIONS_MANAGER_H_
#define _SOP_USERS_PERMISSIONS_MANAGER_H_

#include <string>
#include <boost\shared_ptr.hpp>
#include ".\sop\object.h"
#include ".\sop\users\id_definitions.h"
#include ".\sop\users\permissions.h"
#include ".\sop\temporary.h"

namespace sop
{
  namespace files
  {
    class Inode;
  }

  namespace users
  {
    class Module;

    class PermissionsManager : public Object
    {
      public:
        explicit PermissionsManager(Module *module);
        virtual ~PermissionsManager();
        virtual std::string getClassName() const;

        bool hasPermission(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, permission_t mode);
        bool changePermissions(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, const Permissions & permissions); //has to have permissions
        bool changeINodeUserPermission(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, permission_t mode);
        bool changeINodeGroupPermission(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, permission_t mode);
        bool changeINodeOthersPermission(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, permission_t mode);
        bool changeOwner(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, uid_t new_uid);
        bool changeGroup(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, uid_t new_gid);
        bool isSuperUser(boost::shared_ptr<sop::process::Process> process);

      protected:
        Module *_module;

      private:

    };
  }
}

#endif