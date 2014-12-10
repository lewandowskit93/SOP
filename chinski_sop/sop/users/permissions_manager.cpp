#include ".\sop\users\permissions_manager.h"
#include ".\sop\users\module.h"
#include ".\sop\users\fakers.h"
#include ".\sop\system\kernel.h"

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
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Checking user permissions.");
  if(!node || !process)return false;
  else
  {
    if(isSuperUser(process))return true;
    else if(process->uid==node->uid)
    {
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"User is owner, checking user permission.");
      return PermissionsUtilities::isModeAllowed(node->permissions.user,mode);
    }
    else if(process->gid==node->gid)
    {
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"User is not owner but is in group, checking group permission.");
      return PermissionsUtilities::isModeAllowed(node->permissions.group,mode);
    }
    else
    {
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"User is not owner and not in group, checking user permission.");
      return PermissionsUtilities::isModeAllowed(node->permissions.others,mode);
    }
  }
}

bool sop::users::PermissionsManager::changePermissions(fakers::inode *node, fakers::pcb *process, const Permissions & permissions)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Changing inode permissions.");
  if(hasPermission(node,process,Permissions::kW))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"Permissions changed.");
    node->permissions=permissions;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeUserPermission(fakers::inode *node, fakers::pcb *process, permission_t mode)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Changing inode user permissions.");
  if(hasPermission(node,process,Permissions::kW))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"Permissions changed.");
    node->permissions.user=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeGroupPermission(fakers::inode *node, fakers::pcb *process, permission_t mode)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Changing inode group permissions.");
  if(hasPermission(node,process,Permissions::kW))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"Permissions changed.");
    node->permissions.group=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeOthersPermission(fakers::inode *node, fakers::pcb *process, permission_t mode)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Changing inode others permissions.");
  if(hasPermission(node,process,Permissions::kW))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"Permissions changed.");
    node->permissions.others=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeOwner(fakers::inode *node, fakers::pcb *process, uid_t new_uid)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Changing inode owner.");
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
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Trying change group.");
  if(hasPermission(node,process,Permissions::kW))
  {
    boost::shared_ptr<User> user = _module->getUsersManager()->findUser(process->uid);
    if(!user)return false;
    if(user->gid==new_gid)
    {
      if(!_module->getGroupsManager()->findGroup(new_gid))return false;
      node->gid=new_gid;
      return true;
    }
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINEST,"User not in target group.");
    return false;
  }
  return false;
}

bool sop::users::PermissionsManager::isSuperUser(fakers::pcb *process)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Checking if super user.");
  if(!process) return false;
  return process->uid==0;
}