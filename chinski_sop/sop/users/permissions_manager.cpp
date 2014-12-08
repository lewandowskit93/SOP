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


bool sop::users::PermissionsManager::hasPermission(fakers::inode *node, fakers::pcb *process, permission_t mode)
{
  if(!node || !process)return false;
  else
  {
    if(isSuperUser(process))return true;
    else if(process->uid==node->uid)
    {
      return PermissionsUtilities::isModeAllowed(node->permissions.user,mode);
    }
    else if(process->gid==node->gid)
    {
      return PermissionsUtilities::isModeAllowed(node->permissions.group,mode);
    }
    else
    {
      return PermissionsUtilities::isModeAllowed(node->permissions.others,mode);
    }
  }
}

bool sop::users::PermissionsManager::changePermissions(fakers::inode *node, fakers::pcb *process, const Permissions & permissions)
{
  if(hasPermission(node,process,Permissions::kW))
  {
    node->permissions=permissions;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeUserPermission(fakers::inode *node, fakers::pcb *process, permission_t mode)
{
  if(hasPermission(node,process,Permissions::kW))
  {
    node->permissions.user=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeGroupPermission(fakers::inode *node, fakers::pcb *process, permission_t mode)
{
  if(hasPermission(node,process,Permissions::kW))
  {
    node->permissions.group=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeOthersPermission(fakers::inode *node, fakers::pcb *process, permission_t mode)
{
  if(hasPermission(node,process,Permissions::kW))
  {
    node->permissions.others=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeOwner(fakers::inode *node, fakers::pcb *process, uid_t new_uid)
{
  if(hasPermission(node,process,Permissions::kW))
  {
    if(isSuperUser(process))
    {
      if(!_module->getUsersManager()->findUser(new_uid))return false;
      node->uid=new_uid;
      return true;
    }
    return false;
  }
  return false;
}

bool sop::users::PermissionsManager::changeGroup(fakers::inode *node, fakers::pcb *process, uid_t new_gid)
{
  if(hasPermission(node,process,Permissions::kW))
  {
    boost::shared_ptr<User> user = _module->getUsersManager()->findUser(process->uid);
    if(!user)return false;
    if(user->gid==new_gid)
    {
      if(!_module->getGroupsManager()->findGroup(new_gid))return false; // ToDo: isInGroup(gid,uid) function?
      node->gid=new_gid;
      return true;
    }
    return false;
  }
  return false;
}

bool sop::users::PermissionsManager::isSuperUser(fakers::pcb *process)
{
  if(!process) return false;
  return process->uid==0;
}