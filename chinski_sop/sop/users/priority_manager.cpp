#include ".\sop\users\priority_manager.h"
#include ".\sop\users\module.h"
#include ".\sop\users\user.h"
#include ".\sop\users\users_manager.h"
#include ".\sop\system\kernel.h"
#include ".\sop\logger\logger.h"
#include ".\sop\string_converter.h"

sop::users::PriorityManager::PriorityManager(Module *module):
  sop::Object(),
  _module(module)
{
}

sop::users::PriorityManager::~PriorityManager()
{
}

std::string sop::users::PriorityManager::getClassName() const
{
  return "sop::users::PriorityManager";
}

sop::users::priority_t sop::users::PriorityManager::getUserPriority(uid_t uid)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Asking about user priority. uid="+sop::StringConverter::convertToString<uid_t>(uid));
  boost::shared_ptr<User> user = _module->getUsersManager()->findUser(uid);
  if(!user){
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"User not found. Returning default priority");
    return kDefault_priority;
  }
  else return getGroupPriority(user->gid);
}

sop::users::priority_t sop::users::PriorityManager::getGroupPriority(gid_t gid)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Asking about group priority. gid="
    +sop::StringConverter::convertToString<uid_t>(gid)
    +" returning "
    +sop::StringConverter::convertToString<int16_t>(priorities[gid]));
  return priorities[gid];
}

void sop::users::PriorityManager::setUserPriority(uid_t uid, priority_t priority)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Setting user priority. uid="+sop::StringConverter::convertToString<uid_t>(uid));
  boost::shared_ptr<User> user = _module->getUsersManager()->findUser(uid);
  if(!user)return;
  setGroupPriority(user->gid,priority);
}

void sop::users::PriorityManager::setGroupPriority(gid_t gid, priority_t priority)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Setting group priority to "
    +sop::StringConverter::convertToString<int16_t>(priority)
    +". gid="+sop::StringConverter::convertToString<gid_t>(gid));
  priorities[gid]=(priority < kMin_priority ? kMin_priority : (priority > kMax_priority ? kMax_priority : priority));
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,
    "Priority set to "+sop::StringConverter::convertToString<int16_t>(priorities[gid]));
}

void sop::users::PriorityManager::removeUserPriorityEntry(uid_t uid)
{
  //nothing to do
}

void sop::users::PriorityManager::removeGroupPriorityEntry(gid_t gid)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Removing group priority entry.");
  priorities.erase(gid);
}

void sop::users::PriorityManager::loadPrioritiesFromFile(const std::string & filename)
{
  #ifdef PRIORITIES_FILE_INTEGRATED
  //ToDo: Saving to files module
  #else
  //ToDo: Saving to windows file
  #endif
}

void sop::users::PriorityManager::savePrioritiesToFile(const std::string & filename)
{
  #ifdef PRIORITIES_FILE_INTEGRATED
  //ToDo: Saving to files module
  #else
  //ToDo: Saving to windows file
  #endif
}