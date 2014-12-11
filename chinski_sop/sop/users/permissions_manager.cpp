#include ".\sop\users\permissions_manager.h"
#include ".\sop\users\module.h"
#include ".\sop\system\kernel.h"
#include ".\sop\files\inode.h"
#include ".\sop\temporary.h"

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


bool sop::users::PermissionsManager::hasPermission(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, permission_t mode)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Checking user permissions.");
  if(!node || !process)return false;
  else
  {
    if(isSuperUser(process))return true;
    else if(process->uid==node->uid)
    {
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"User is owner, checking user permission.");
      return PermissionsUtilities::isModeAllowed(node->getPermissions().user,mode);
    }
    else if(process->gid==node->gid)
    {
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"User is not owner but is in group, checking group permission.");
      return PermissionsUtilities::isModeAllowed(node->getPermissions().group,mode);
    }
    else
    {
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"User is not owner and not in group, checking user permission.");
      return PermissionsUtilities::isModeAllowed(node->getPermissions().others,mode);
    }
  }
}

bool sop::users::PermissionsManager::changePermissions(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, Permissions permissions)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Changing inode permissions.");
  if(hasPermission(node,process,Permissions::kW))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"Permissions changed.");
    node->getPermissions().user=permissions.user;
    node->getPermissions().group=permissions.group;
    node->getPermissions().others=permissions.others;
    std::cout<<node<<std::endl;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeUserPermission(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, permission_t mode)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Changing inode user permissions.");
  if(hasPermission(node,process,Permissions::kW))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"Permissions changed.");
    node->getPermissions().user=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeGroupPermission(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, permission_t mode)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Changing inode group permissions.");
  if(hasPermission(node,process,Permissions::kW))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"Permissions changed.");
    node->getPermissions().group=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeINodeOthersPermission(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, permission_t mode)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Changing inode others permissions.");
  if(hasPermission(node,process,Permissions::kW))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"Permissions changed.");
    node->getPermissions().others=mode;
    return true;
  }
  return false;
}

bool sop::users::PermissionsManager::changeOwner(sop::files::Inode *node, boost::shared_ptr<sop::process::Process> process, uid_t new_uid)
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

bool sop::users::PermissionsManager::changeGroup(sop::files::Inode *node,boost::shared_ptr<sop::process::Process> process, uid_t new_gid)
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

bool sop::users::PermissionsManager::isSuperUser(boost::shared_ptr<sop::process::Process> process)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Checking if super user.");
  if(!process) return false;
  return process->uid==0;
}