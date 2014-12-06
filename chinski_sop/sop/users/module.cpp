#include <iostream>
#include ".\sop\users\module.h"
#include ".\sop\users\user.h"
#include ".\sop\users\id_definitions.h"
#include ".\sop\system\kernel.h"

sop::users::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel),
  _users_manager(this)
{

}

sop::users::Module::~Module()
{

}

std::string sop::users::Module::getClassName() const
{
  return "sop::users::Module";
}

void sop::users::Module::initialize()
{
  getKernel()->getShell()->registerCommand("useradd",&Module::cH_useradd,this);
  getKernel()->getShell()->registerCommand("userslist",&Module::cH_userslist,this);
  getKernel()->getShell()->registerCommand("userdel",&Module::cH_userdel,this);
  getKernel()->getShell()->registerCommand("userfind",&Module::cH_userfind,this);
}

sop::users::UsersManager* sop::users::Module::getUsersManager()
{
  return &_users_manager;
}

bool sop::users::Module::hasParam(const std::vector<const std::string> & param, const std::string & param_name)
{
  for(std::vector<std::string>::size_type i = 0; i<param.size();++i)
  {
    if(param[i]==param_name) return true;
  }
  return false;
}

std::string sop::users::Module::getParamValue(const std::vector<const std::string> & param, const std::string & param_name)
{
  for(std::vector<std::string>::size_type i = 0; i<(param.size()-1);++i)
  {
    if(param[i]==param_name)
    {
      return param[i+1];
    }
  }
  return "";
}

void sop::users::Module::cH_useradd(const std::vector<const std::string> & params)
{
  if(hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"useradd [-u uid] [-g gid] [-p passwd] [-c comment] [-d home_dir] [-h] login"<<std::endl;
    std::cout<<"Creates an user specified by given parameters."<<std::endl;
  }
  else
  {
    User user;

    if(hasParam(params,"-u"))
    {
      user.uid = convertStringTo<uid_t>(getParamValue(params,"-u"));
    }
    else
    {
      user.uid=_users_manager.getNextFreeUID();
    }

    if(hasParam(params,"-g"))
    {
      // ToDo: group must exist already...
      user.gid = convertStringTo<uid_t>(getParamValue(params,"-g"));
    }
    else
    {
      // ToDo: check if gid=uid available if yes - ok, if no, take next free...
      // and also - check if group name is availble if not, there is no possibility to create the user
      user.gid=user.uid;
    }

    if(hasParam(params, "-p"))
    {
      // ToDo: encrypt password
      user.password = getParamValue(params,"-p");
    }
    
    if(hasParam(params,"-c"))
    {
      user.info = getParamValue(params,"-c");
    }


    user.username = params[params.size()-1];

    if(hasParam(params,"-d"))
    {
      user.home_dir=getParamValue(params,"-d");
    }
    else
    {
      user.home_dir="home\\"+user.username;
    }

    if(_users_manager.addUser(user))
    {
      // ToDo: create user dir if not exist, add user entry to file, and also group entry if it is new group (if new, create one)
    }
  }
}

void sop::users::Module::cH_userfind(const std::vector<const std::string> & params)
{
  boost::shared_ptr<User> user;
  if(hasParam(params,"-u"))
  {
    user = _users_manager.findUser(convertStringTo<uid_t>(getParamValue(params,"-u")));
  }
  else if(hasParam(params,"-l"))
  {
    user = _users_manager.findUser(getParamValue(params,"-l"));
  }
  else
  {
    std::cout<<"userfind [-h] [-l username] [-u uid]"<<std::endl;
    std::cout<<"Finds user specified by uid or login and prints info about it"<<std::endl;
    return;
  }

  if(user)
  {
    std::cout<<"UID\t|LOGIN\t|GID\t|INFO\t|HOME DIR\t|PASSWORD PROTECTION"<<std::endl;
    std::cout<<user->uid<<"\t|"<<user->username<<"\t|"<<user->gid<<"\t|"<<user->info<<"\t|"<<user->home_dir<<"\t|"<<(user->password==""? "N":"Y")<<std::endl;
  }
  else
  {
    std::cout<<"User not found"<<std::endl;
  }
}

void sop::users::Module::cH_userdel(const std::vector<const std::string> & params)
{
  if(hasParam(params, "-h") || params.size()==1)
  {
    std::cout<<"userdel [-h] login"<<std::endl;
    std::cout<<"Deletes user specified by the given username"<<std::endl;
  }
  else
  {
    // ToDo: delete also group if the user was the last user in this group (and gid==uid)
    // delete home dir of that user if he is only owner
    // user cannot be logged in
    // save the file
    _users_manager.deleteUser(params[params.size()-1]);
  }
}

void sop::users::Module::cH_userslist(const std::vector<const std::string> & params)
{
  if(params.size()>1)
  {
    std::cout<<"userslist [-h]"<<std::endl;
    std::cout<<"Lists all users in the system"<<std::endl;
    return;
  }
  
  std::list<boost::shared_ptr<User>> users_list = _users_manager.getUsersList();
  std::list<boost::shared_ptr<User>>::iterator it;
  std::cout<<"UID\t|LOGIN\t|GID\t|INFO\t|HOME DIR\t|PASSWORD PROTECTION"<<std::endl;
  for(it=users_list.begin();it!=users_list.end();++it)
  {
    std::cout<<(*it)->uid<<"\t|"<<(*it)->username<<"\t|"<<(*it)->gid<<"\t|"<<(*it)->info<<"\t|"<<(*it)->home_dir<<"\t|"<<((*it)->password==""? "N":"Y")<<std::endl;
  }
}