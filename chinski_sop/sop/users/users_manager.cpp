#include <boost\regex.hpp>
#include ".\sop\users\users_manager.h"
#include ".\sop\users\module.h"
#include ".\sop\system\kernel.h"
#include ".\sop\system\shell.h"
#include ".\sop\logger\logger.h"
#include ".\sop\string_converter.h"

const boost::regex sop::users::UsersManager::username_regex = boost::regex("^[a-zA-Z][0-9a-zA-Z_]*$");

sop::users::UsersManager::UsersManager(Module *module):
  sop::Object(),
  _module(module)  
{
  _users_list.push_back(nobody);
}

sop::users::UsersManager::~UsersManager()
{
}

std::string sop::users::UsersManager::getClassName() const
{
  return "UsersManager";
}


bool sop::users::UsersManager::addUser(boost::shared_ptr<User> user)
{
  if(!user)return false;

  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Asked to create user");

  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"uid: "+sop::StringConverter::convertToString<uid_t>(user->uid));
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"username: "+user->username);
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINER,"gid: "+sop::StringConverter::convertToString<gid_t>(user->gid));

  if(!isUsernameValid(user->username))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"User creation failed. Invalid username.");
    return false;
  }

  if(isUIDFree(user->uid) && isUsernameFree(user->username))
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"User created");
    std::list<boost::shared_ptr<User>>::iterator where_to_insert=_users_list.begin();
    for(;where_to_insert!=_users_list.end();++where_to_insert)
    {
      if((*where_to_insert)->uid>user->uid)break;
    }
    _users_list.insert(where_to_insert,user);
    return true;
  }
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"User creation failed");
  return false;
}

bool sop::users::UsersManager::addUser(const User & user)
{
  boost::shared_ptr<User> user_ptr(new User(user));
  return addUser(user_ptr);
}

bool sop::users::UsersManager::deleteUser(const std::string & username)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Deleting user");
  if(username=="root" || username=="nobody")
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Cannot delete this user");
    return false;
  }
  std::list<boost::shared_ptr<User>>::iterator to_erase = _users_list.end();
  std::list<boost::shared_ptr<User>>::iterator it;
  for(it=_users_list.begin();it!=_users_list.end();++it)
  {
    if((*it)->username == username)
    {
      to_erase = it;
      break;
    }
  }
  if(to_erase==_users_list.end())
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Unknown user");
    return false;
  }
  else
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"User deleted");
    _users_list.erase(to_erase);
    return true;
  }
}

boost::shared_ptr<sop::users::User> sop::users::UsersManager::findUser(uid_t uid)
{
  boost::shared_ptr<User> user_found;
  std::list<boost::shared_ptr<User>>::iterator it;
  for(it=_users_list.begin();it!=_users_list.end();++it)
  {
    if((*it)->uid == uid)
    {
      user_found=(*it);
      break;
    }
  }
  return user_found;
}

boost::shared_ptr<sop::users::User> sop::users::UsersManager::findUser(const std::string & username)
{
  boost::shared_ptr<User> user_found;
  std::list<boost::shared_ptr<User>>::iterator it;
  for(it=_users_list.begin();it!=_users_list.end();++it)
  {
    if((*it)->username == username)
    {
      user_found=(*it);
      break;
    }
  }
  return user_found;
}

sop::users::uid_t sop::users::UsersManager::getNextFreeUID(uid_t greater_than)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Calculating free UID");
  uid_t last_uid = greater_than;
  uid_t free_uid = nobody->uid;
  bool found=false;
  std::list<boost::shared_ptr<User>>::iterator it;

  for(it=_users_list.begin();it!=_users_list.end();++it)
  {
    if(it == _users_list.begin())
    {
      last_uid=(*it)->uid;
    }
    else
    {
      if(last_uid<=greater_than)
      {
        if((*it)->uid < greater_than) continue;
        if(((*it)->uid - greater_than) > 1)
        {
          free_uid=greater_than+1;
          found=true;
          break;
        }
      }
      else if(((*it)->uid-last_uid) > 1)
      {
        free_uid=last_uid+1;
        found=true;
        break;
      }
      last_uid=(*it)->uid;
    }
  }
  
  if(!found)
  {
    if(_users_list.size()==0)
    {
      free_uid=greater_than+1;
    }
    else if(_users_list.size()==1)
    {
      if(last_uid<=greater_than)free_uid=greater_than+1;
      else if(last_uid-greater_than > 1) free_uid=greater_than+1;
      else free_uid=last_uid+1;
    }
    else if(last_uid<=greater_than)free_uid=greater_than+1;
    else free_uid=last_uid+1;
  }

  if(free_uid>nobody->uid)free_uid=nobody->uid;

  return free_uid;
}

bool sop::users::UsersManager::isUIDFree(uid_t uid)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Checking if UID is free");
  if(uid>=nobody->uid){
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"UID is taken");
    return false;
  }
  std::list<boost::shared_ptr<User>>::iterator it;
  for(it=_users_list.begin();it!=_users_list.end();++it)
  {
    if((*it)->uid == uid){
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"UID is taken");
      return false;
    }
  }
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"UID is free");
  return true;
}

bool sop::users::UsersManager::isUsernameFree(const std::string & username)
{
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Checking if username is free");
  if(username==nobody->username)
  {
    _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Username is taken");
    return false;
  }
  std::list<boost::shared_ptr<User>>::iterator it;
  for(it=_users_list.begin();it!=_users_list.end();++it)
  {
    if((*it)->username == username)
    {
      _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::INFO,"Username is taken");
      return false;
    }
  }
  _module->getKernel()->getLogger()->logUsers(sop::logger::Logger::Level::FINE,"Username is free");
  return true;
}

void sop::users::UsersManager::loadUsersFromFile(const std::string & filename)
{
#ifdef PASSWD_FILE_INTEGRATED
  //ToDo: Loading from files module
#else
  //ToDo: Loading from windows file
#endif
}

void sop::users::UsersManager::saveUsersToFile(const std::string & filename)
{
#ifdef PASSWD_FILE_INTEGRATED
  //ToDo: Saving to files module
#else
  //ToDo: Saving to windows file
#endif
}

std::list<boost::shared_ptr<sop::users::User>> sop::users::UsersManager::getUsersList()
{
  return _users_list;
}

bool sop::users::UsersManager::isUsernameValid(const std::string & username)
{
  return boost::regex_match(username,username_regex);
}