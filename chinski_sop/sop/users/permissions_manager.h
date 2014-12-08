#ifndef _SOP_USERS_PERMISSIONS_MANAGER_H_
#define _SOP_USERS_PERMISSIONS_MANAGER_H_

#include <string>
#include ".\sop\object.h"
#include ".\sop\users\permissions.h"

namespace sop
{
  namespace users
  {
    class Module;
    struct pcb;
    struct inode;

    class PermissionsManager : public Object
    {
      public:
        explicit PermissionsManager(Module *module);
        virtual ~PermissionsManager();
        virtual std::string getClassName() const;

        bool hasPermission(inode *node, pcb *process, permission_t mode);
        bool changePermissions(inode *node, pcb *process, const Permissions & permissions); //has to have permissions
        bool changeINodeUserPermission(inode *node, pcb *process, permission_t mode);
        bool changeINodeGroupPermission(inode *node, pcb *process, permission_t mode);
        bool changeINodeOthersPermission(inode *node, pcb *process, permission_t mode);

      protected:
        Module *_module;

      private:

    };
  }
}

#endif