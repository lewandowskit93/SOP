#include ".\sop\users\permissions_manager.h"
#include ".\sop\users\module.h"
#include ".\sop\users\fakers.h"

sop::users::PermissionsManager::PermissionsManager(Module *module):
  Object(),
  _module(module)
{

}

sop::users::PermissionsManager::~PermissionsManager()
{

}

std::string sop::users::PermissionsManager::getClassName() const
{
  return "sop::users::PermissionsManager";
}


bool sop::users::PermissionsManager::hasPermission(inode *node, pcb *process, permission_t mode)
{
  //ToDo: check permissions
  return false;
}

bool sop::users::PermissionsManager::changePermissions(inode *node, pcb *process, const Permissions & permissions)
{
  if(hasPermission(node,process,Permissions::kW))
  {
    node->permissions=permissions;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeUserPermission(inode *node, pcb *process, permission_t mode)
{
  if(hasPermission(node,process,Permissions::kW))
  {
    node->permissions.user=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeGroupPermission(inode *node, pcb *process, permission_t mode)
{
  if(hasPermission(node,process,Permissions::kW))
  {
    node->permissions.group=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeOthersPermission(inode *node, pcb *process, permission_t mode)
{
  if(hasPermission(node,process,Permissions::kW))
  {
    node->permissions.others=mode;
    return true;
  }
  return false;
}
