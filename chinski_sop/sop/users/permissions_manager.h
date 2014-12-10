#ifndef _SOP_USERS_PERMISSIONS_MANAGER_H_
#define _SOP_USERS_PERMISSIONS_MANAGER_H_

#include <string>
#include <boost\shared_ptr.hpp>
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

        bool hasPermission(fakers::inode *node, boost::shared_ptr<fakers::pcb> process, permission_t mode);
        bool changePermissions(fakers::inode *node, boost::shared_ptr<fakers::pcb> process, const Permissions & permissions); //has to have permissions
        bool changeINodeUserPermission(fakers::inode *node, boost::shared_ptr<fakers::pcb> process, permission_t mode);
        bool changeINodeGroupPermission(fakers::inode *node, boost::shared_ptr<fakers::pcb> process, permission_t mode);
        bool changeINodeOthersPermission(fakers::inode *node, boost::shared_ptr<fakers::pcb> process, permission_t mode);
        bool changeOwner(fakers::inode *node, boost::shared_ptr<fakers::pcb> process, uid_t new_uid);
        bool changeGroup(fakers::inode *node, boost::shared_ptr<fakers::pcb> process, uid_t new_gid);
        bool isSuperUser(boost::shared_ptr<fakers::pcb> process);

      protected:
        Module *_module;

      private:

    };
  }
}

#endif