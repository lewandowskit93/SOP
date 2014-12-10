#include <boost\regex.hpp>
#include ".\sop\users\groups_manager.h"
#include ".\sop\users\module.h"
#include ".\sop\system\kernel.h"
#include ".\sop\system\shell.h"
#include ".\sop\logger\logger.h"
#include ".\sop\string_converter.h"

const boost::regex sop::users::GroupsManager::group_name_regex = boost::regex("^[a-zA-Z][0-9a-zA-Z_]*$");

sop::users::GroupsManager::GroupsManager(Module *module):
  sop::Object(),
  _module(module)  
{
  nogroup->users_list.push_back(nobody);
  _groups_list.push_back(nogroup);
}

sop::users::GroupsManager::~GroupsManager()
{
}

std::string sop::users::GroupsManager::getClassName() const
{
  return "GroupsManager";
}


bool sop::users::GroupsManager::addGroup(boost::shared_ptr<Group> group)
{
  if(!group)return false;

  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Asked to create group");

  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"gid: "+sop::StringConverter::convertToString<gid_t>(group->gid));
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"group_name: "+group->group_name);

  if(!isGroupNameValid(group->group_name))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Group creation failed. Invalid group name.");
    return false;
  }

  if(isGIDFree(group->gid) && isGroupNameFree(group->group_name))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Group created");
    std::list<boost::shared_ptr<Group>>::iterator where_to_insert=_groups_list.begin();
    for(;where_to_insert!=_groups_list.end();++where_to_insert)
    {
      if((*where_to_insert)->gid>group->gid)break;
    }
    _groups_list.insert(where_to_insert,group);
    return true;
  }
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Group creation failed");
  return false;
}

bool sop::users::GroupsManager::addGroup(const Group & group)
{
  boost::shared_ptr<Group> group_ptr(new Group(group));
  return addGroup(group_ptr);
}

bool sop::users::GroupsManager::deleteGroup(const std::string & group_name)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Deleting group");
  if(group_name=="root" || group_name=="nogroup")
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Cannot delete this group");
    return false;
  }
  std::list<boost::shared_ptr<Group>>::iterator to_erase = _groups_list.end();
  std::list<boost::shared_ptr<Group>>::iterator it;
  for(it=_groups_list.begin();it!=_groups_list.end();++it)
  {
    if((*it)->group_name == group_name)
    {
      to_erase = it;
      break;
    }
  }
  if(to_erase==_groups_list.end())
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Unknown group");
    return false;
  }
  else if((*to_erase)->users_list.size()>0)
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Group not empty");
    return false;
  }
  else
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"Group deleted");
    _groups_list.erase(to_erase);
    return true;
  }
}

boost::shared_ptr<sop::users::Group> sop::users::GroupsManager::findGroup(gid_t gid)
{
  boost::shared_ptr<Group> group_found;
  std::list<boost::shared_ptr<Group>>::iterator it;
  for(it=_groups_list.begin();it!=_groups_list.end();++it)
  {
    if((*it)->gid == gid)
    {
      group_found=(*it);
      break;
    }
  }
  return group_found;
}

boost::shared_ptr<sop::users::Group> sop::users::GroupsManager::findGroup(const std::string & group_name)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Looking for group.");
  boost::shared_ptr<Group> group_found;
  std::list<boost::shared_ptr<Group>>::iterator it;
  for(it=_groups_list.begin();it!=_groups_list.end();++it)
  {
    if((*it)->group_name == group_name)
    {
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINEST,"Group found.");
      group_found=(*it);
      break;
    }
  }
  if(!group_found)_module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINEST,"Group not found.");
  return group_found;
}

sop::users::gid_t sop::users::GroupsManager::getNextFreeGID(gid_t greater_than)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Calculating free GID");
  gid_t last_gid = greater_than;
  gid_t free_gid = nogroup->gid;
  bool found=false;
  std::list<boost::shared_ptr<Group>>::iterator it;

  for(it=_groups_list.begin();it!=_groups_list.end();++it)
  {
    if(it == _groups_list.begin())
    {
      last_gid=(*it)->gid;
    }
    else
    {
      if(last_gid<=greater_than)
      {
        if((*it)->gid < greater_than) continue;
        if(((*it)->gid - greater_than) > 1)
        {
          free_gid=greater_than+1;
          found=true;
          break;
        }
      }
      else if(((*it)->gid-last_gid) > 1)
      {
        free_gid=last_gid+1;
        found=true;
        break;
      }
      last_gid=(*it)->gid;
    }
  }
  
  if(!found)
  {
    if(_groups_list.size()==0)
    {
      free_gid=greater_than+1;
    }
    else if(_groups_list.size()==1)
    {
      if(last_gid<=greater_than)free_gid=greater_than+1;
      else if(last_gid-greater_than > 1) free_gid=greater_than+1;
      else free_gid=last_gid+1;
    }
    else if(last_gid<=greater_than)free_gid=greater_than+1;
    else free_gid=last_gid+1;
  }

  if(free_gid>nogroup->gid)free_gid=nogroup->gid;

  return free_gid;
}

bool sop::users::GroupsManager::isGIDFree(gid_t gid)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Checking if GID is free");
  if(gid>=nogroup->gid){
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"GID is taken");
    return false;
  }
  std::list<boost::shared_ptr<Group>>::iterator it;
  for(it=_groups_list.begin();it!=_groups_list.end();++it)
  {
    if((*it)->gid == gid){
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"GID is taken");
      return false;
    }
  }
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"GID is free");
  return true;
}

bool sop::users::GroupsManager::isGroupNameFree(const std::string & group_name)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Checking if group_name is free");
  if(group_name==nogroup->group_name)
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"GroupName is taken");
    return false;
  }
  std::list<boost::shared_ptr<Group>>::iterator it;
  for(it=_groups_list.begin();it!=_groups_list.end();++it)
  {
    if((*it)->group_name == group_name)
    {
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"GroupName is taken");
      return false;
    }
  }
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"GroupName is free");
  return true;
}

void sop::users::GroupsManager::loadGroupsFromFile(const std::string & filename)
{
#ifdef GROUP_FILE_INTEGRATED
  //ToDo: Loading from files module
#else
  //ToDo: Loading from windows file
#endif
}

void sop::users::GroupsManager::saveGroupsToFile(const std::string & filename)
{
#ifdef GROUP_FILE_INTEGRATED
  //ToDo: Saving to files module
#else
  //ToDo: Saving to windows file
#endif
}

std::list<boost::shared_ptr<sop::users::Group>> sop::users::GroupsManager::getGroupsList()
{
  return _groups_list;
}

bool sop::users::GroupsManager::isGroupNameValid(const std::string & group_name)
{
  return boost::regex_match(group_name,group_name_regex);
}