#ifndef _SOP_USERS_PERMISSIONS_MANAGER_H_
#define _SOP_USERS_PERMISSIONS_MANAGER_H_

#include <string>
#include ".\sop\object.h"
#include ".\sop\users\id_definitions.h"
#include ".\sop\users\permissions.h"

namespace sop
{
  namespace users
  {
    class Module;
    namespace fakers
    {
      struct pcb;
      struct inode;
    }

    class PermissionsManager : public Object
    {
      public:
        explicit PermissionsManager(Module *module);
        virtual ~PermissionsManager();
        virtual std::string getClassName() const;

        bool hasPermission(fakers::inode *node, fakers::pcb *process, permission_t mode);
        bool changePermissions(fakers::inode *node, fakers::pcb *process, const Permissions & permissions); //has to have permissions
        bool changeINodeUserPermission(fakers::inode *node, fakers::pcb *process, permission_t mode);
        bool changeINodeGroupPermission(fakers::inode *node, fakers::pcb *process, permission_t mode);
        bool changeINodeOthersPermission(fakers::inode *node, fakers::pcb *process, permission_t mode);
        bool changeOwner(fakers::inode *node, fakers::pcb *process, uid_t new_uid);
        bool changeGroup(fakers::inode *node, fakers::pcb *process, uid_t new_gid);
        bool isSuperUser(fakers::pcb *process);

      protected:
        Module *_module;

      private:

    };
  }
}

#endif