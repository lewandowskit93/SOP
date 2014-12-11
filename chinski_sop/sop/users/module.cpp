#include <iostream>
#include ".\sop\users\module.h"
#include ".\sop\users\user.h"
#include ".\sop\users\id_definitions.h"
#include ".\sop\system\kernel.h"
#include ".\sop\system\shell.h"
#include ".\sop\string_converter.h"
#include ".\sop\users\encryptors.h"
#include ".\sop\files\inode.h"
#include ".\sop\files\filesystem.h"
#include ".\sop\temporary.h"

sop::users::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel),
  _users_manager(this),
  _groups_manager(this),
  _priority_manager(this),
  _permissions_manager(this)
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
  // ToDo: load users from list (if there is no list, then create root)
  boost::shared_ptr<User> root_usr(new User(0,0,"root","","Superuser","/home/root/"));
  boost::shared_ptr<Group> root_group(new Group(0,"root"));
  root_group->users_list.push_back(root_usr);
  _users_manager.addUser(root_usr);
  _groups_manager.addGroup(root_group);
  getKernel()->getShell()->registerCommand("useradd",&Module::cH_useradd,this);
  getKernel()->getShell()->registerCommand("userslist",&Module::cH_userslist,this);
  getKernel()->getShell()->registerCommand("userdel",&Module::cH_userdel,this);
  getKernel()->getShell()->registerCommand("userfind",&Module::cH_userfind,this);
  getKernel()->getShell()->registerCommand("chpasswd",&Module::cH_chpasswd,this);
  getKernel()->getShell()->registerCommand("groupadd",&Module::cH_groupadd,this);
  getKernel()->getShell()->registerCommand("groupdel",&Module::cH_groupdel,this);
  getKernel()->getShell()->registerCommand("groupfind",&Module::cH_groupfind,this);
  getKernel()->getShell()->registerCommand("groupmembers",&Module::cH_groupmembers,this);
  getKernel()->getShell()->registerCommand("groupslist",&Module::cH_groupslist,this);
  getKernel()->getShell()->registerCommand("groupchange",&Module::cH_groupchange,this);
  getKernel()->getShell()->registerCommand("nice",&Module::cH_nice,this);
  getKernel()->getShell()->registerCommand("shownice",&Module::cH_shownice,this);
  getKernel()->getShell()->registerCommand("removeniceentry",&Module::cH_removeniceentry,this);
  getKernel()->getShell()->registerCommand("login",&Module::cH_login,this);
  getKernel()->getShell()->registerCommand("whois",&Module::cH_whois,this);
  getKernel()->getShell()->registerCommand("access",&Module::cH_access,this);

  getKernel()->getShell()->registerCommand("chmod",&Module::cH_chmod,this);
  getKernel()->getShell()->registerCommand("chown",&Module::cH_chown,this);
  getKernel()->getShell()->registerCommand("chgrp",&Module::cH_chgrp,this);
}

sop::users::UsersManager* sop::users::Module::getUsersManager()
{
  return &_users_manager;
}

sop::users::GroupsManager* sop::users::Module::getGroupsManager()
{
  return &_groups_manager;
}

sop::users::PriorityManager* sop::users::Module::getPriorityManager()
{
  return &_priority_manager;
}

sop::users::PermissionsManager* sop::users::Module::getPermissionsManager()
{
  return &_permissions_manager;
}

void sop::users::Module::cH_useradd(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"useradd [-u uid] [-g gid] [-p passwd] [-c comment] [-d home_dir] [-h] login"<<std::endl;
    std::cout<<"Creates an user specified by given parameters."<<std::endl;
  }
  else
  {
    if(!_permissions_manager.isSuperUser(sop::process::getProcess(0)))
    {
      std::cout<<"Not a super user."<<std::endl;
      return;
    }

    User user;

    if(sop::system::Shell::hasParam(params,"-u"))
    {
      user.uid = sop::StringConverter::convertStringTo<uid_t>(sop::system::Shell::getParamValue(params,"-u"));
    }
    else
    {
      user.uid=_users_manager.getNextFreeUID();
    }

    if(sop::system::Shell::hasParam(params,"-g"))
    {
      user.gid = sop::StringConverter::convertStringTo<uid_t>(sop::system::Shell::getParamValue(params,"-g"));
      boost::shared_ptr<Group> group = _groups_manager.findGroup(user.gid);
      if(!group)return;
      else if(group->group_name=="nogroup")return;
    }
    else
    {
      if(!_groups_manager.isGroupNameFree(user.username))return;
      if(_groups_manager.isGIDFree(user.uid))user.gid=user.uid;
      else{
        user.gid=_groups_manager.getNextFreeGID();
      }
    }

    if(sop::system::Shell::hasParam(params, "-p"))
    {
      user.password = sop::system::Shell::getParamValue(params,"-p");
      if(!sop::users::UsersManager::checkPasswordFormat(user.password))
      {
        std::cout<<"Invalid password format."<<std::endl;
        return;
      }
      user.password = _users_manager.getEncryptor()->encrypt(user.password);
    }
    
    if(sop::system::Shell::hasParam(params,"-c"))
    {
      user.info = sop::system::Shell::getParamValue(params,"-c");
    }


    user.username = params[params.size()-1];

    if(sop::system::Shell::hasParam(params,"-d"))
    {
      user.home_dir=sop::system::Shell::getParamValue(params,"-d");
    }
    else
    {
      user.home_dir="/home/"+user.username+"/";
    }

    if(_users_manager.addUser(user))
    {
      if(!sop::system::Shell::hasParam(params,"-g"))
      {
        Group group(user.gid,user.username);
        group.users_list.push_back(_users_manager.findUser(user.uid));
        if(!_groups_manager.addGroup(group))
        {
          _users_manager.deleteUser(user.username);
          return;
        }
      }
      else
      {
        boost::shared_ptr<Group> group = _groups_manager.findGroup(user.gid);
        group->users_list.push_back(_users_manager.findUser(user.uid));
      }
      // ToDo: create user dir if not exist, save users and groups files
    }
  }
}

void sop::users::Module::cH_userfind(const std::vector<const std::string> & params)
{
  boost::shared_ptr<User> user;
  if(sop::system::Shell::hasParam(params,"-u"))
  {
    user = _users_manager.findUser(sop::StringConverter::convertStringTo<uid_t>(sop::system::Shell::getParamValue(params,"-u")));
  }
  else if(sop::system::Shell::hasParam(params,"-l"))
  {
    user = _users_manager.findUser(sop::system::Shell::getParamValue(params,"-l"));
  }
  else
  {
    std::cout<<"userfind [-h] [-l username] [-u uid]"<<std::endl;
    std::cout<<"Finds user specified by uid or login and prints info about it"<<std::endl;
    return;
  }

  if(user)
  {
    std::cout<<"UID\t|LOGIN\t|GID\t|INFO\t|HOME DIR\t|ENCRYPTED_PASSWORD"<<std::endl;
    std::cout<<user->uid<<"\t|"<<user->username<<"\t|"<<user->gid<<"\t|"<<user->info<<"\t|"<<user->home_dir<<"\t|"<<user->password<<std::endl;
  }
  else
  {
    std::cout<<"User not found"<<std::endl;
  }
}

void sop::users::Module::cH_userdel(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params, "-h") || params.size()==1)
  {
    std::cout<<"userdel [-h] login"<<std::endl;
    std::cout<<"Deletes user specified by the given username"<<std::endl;
  }
  else
  {
    // ToDo:
    // save the file
    if(!_permissions_manager.isSuperUser(sop::process::getProcess(0)))
    {
      std::cout<<"Not a super user."<<std::endl;
      return;
    }

    boost::shared_ptr<User> user = _users_manager.findUser(params[params.size()-1]);
    if(sop::process::getProcess(0)->uid==user->uid)
    {
      std::cout<<"User cannot be logged in."<<std::endl;
      return;
    }
    if(user)
    {
      boost::shared_ptr<Group> group = _groups_manager.findGroup(user->gid);
      if(group && _users_manager.deleteUser(user->username))
      {
        group->users_list.remove(user);
        //if(group->users_list.size()==0)_groups_manager.deleteGroup(group->group_name); // do not delete group to allow empty groups to exist
      }
    }
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

void sop::users::Module::cH_chpasswd(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params, "-h") || params.size()==1)
  {
    std::cout<<"chpasswd [-h] [-o old_password] [-n new_password] login"<<std::endl;
    std::cout<<"Changes user password."<<std::endl;
  }
  else
  {
    boost::shared_ptr<User> user = _users_manager.findUser(params[params.size()-1]);
    std::string old_pass = "";
    std::string new_pass = "";
    if(sop::system::Shell::hasParam(params, "-o"))
    {
      old_pass=sop::system::Shell::getParamValue(params, "-o");
    }
    if(sop::system::Shell::hasParam(params, "-n"))
    {
      new_pass=sop::system::Shell::getParamValue(params, "-n");
      if(!sop::users::UsersManager::checkPasswordFormat(new_pass))
      {
        std::cout<<"Invalid password format."<<std::endl;
        return;
      }
      new_pass = _users_manager.getEncryptor()->encrypt(new_pass);
    }

    if(user)
    {
      if(_users_manager.isPasswordValid(user,old_pass))user->password=new_pass; // ToDo: save file
      else std::cout<<"Wrong password."<<std::endl;
    }
    else std::cout<<"Invalid user."<<std::endl;

  }
}

void sop::users::Module::cH_groupadd(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params, "-h") || params.size()==1)
  {
    std::cout<<"groupadd [-h] [-g gid] group_name"<<std::endl;
    std::cout<<"Creates group specified by group_name."<<std::endl;
  }
  else
  {
    if(!_permissions_manager.isSuperUser(sop::process::getProcess(0)))
    {
      std::cout<<"Not a super user."<<std::endl;
      return;
    }
    Group group;
    group.group_name=params[params.size()-1];
    if(sop::system::Shell::hasParam(params, "-g"))
    {
      group.gid=sop::StringConverter::convertStringTo<gid_t>(sop::system::Shell::getParamValue(params,"-g"));
    }
    else
    {
      group.gid=_groups_manager.getNextFreeGID();
    }

    _groups_manager.addGroup(group);
    // ToDo: save file
  }
}

void sop::users::Module::cH_groupfind(const std::vector<const std::string> & params)
{
  boost::shared_ptr<Group> group;
  if(sop::system::Shell::hasParam(params,"-g"))
  {
    group = _groups_manager.findGroup(sop::StringConverter::convertStringTo<gid_t>(sop::system::Shell::getParamValue(params,"-g")));
  }
  else if(sop::system::Shell::hasParam(params,"-n"))
  {
    group = _groups_manager.findGroup(sop::system::Shell::getParamValue(params,"-n"));
  }
  else
  {
    std::cout<<"userfind [-h] [-n username] [-g gid]"<<std::endl;
    std::cout<<"Finds group specified by gid or group name and prints info about it."<<std::endl;
    return;
  }

  if(group)
  {
    std::cout<<"GID\t|GROUP NAME\t|NR OF USERS"<<std::endl;
    std::cout<<group->gid<<"\t|"<<group->group_name<<"\t|"<<group->users_list.size()<<std::endl;
  }
  else
  {
    std::cout<<"Group not found"<<std::endl;
  }
}

void sop::users::Module::cH_groupdel(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params, "-h") || params.size()==1)
  {
    std::cout<<"groupdel [-h] group_name"<<std::endl;
    std::cout<<"Group specified by the given group_name (if it is empty)"<<std::endl;
  }
  else
  {
    if(!_permissions_manager.isSuperUser(sop::process::getProcess(0)))
    {
      std::cout<<"Not a super user."<<std::endl;
      return;
    }
    _groups_manager.deleteGroup(params[params.size()-1]);
    // ToDo: save the file
  }
}

void sop::users::Module::cH_groupslist(const std::vector<const std::string> & params)
{
  if(params.size()>1)
  {
    std::cout<<"groupslist [-h]"<<std::endl;
    std::cout<<"Lists all groups in the system"<<std::endl;
    return;
  }
  
  std::list<boost::shared_ptr<Group>> groups_list = _groups_manager.getGroupsList();
  std::list<boost::shared_ptr<Group>>::iterator it;
  std::cout<<"GID\t|GROUP NAME\t|NR OF USERS"<<std::endl;
  for(it=groups_list.begin();it!=groups_list.end();++it)
  {
    std::cout<<(*it)->gid<<"\t|"<<(*it)->group_name<<"\t|"<<(*it)->users_list.size()<<std::endl;
  }
}

void sop::users::Module::cH_groupmembers(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"groupmembers [-u] [-h] group_name"<<std::endl;
    std::cout<<"Lists all members of the given group"<<std::endl;
    return;
  }
  else
  {
    bool uids = sop::system::Shell::hasParam(params,"-u");
    boost::shared_ptr<Group> group = _groups_manager.findGroup(params[params.size()-1]);
    if(!group)
    {
      std::cout<<"Group not found"<<std::endl;
    }
    else
    {
      std::cout<<"Members of group: "<<params[params.size()-1]<<std::endl;
      std::list<boost::shared_ptr<User>>::iterator it = group->users_list.begin();
      for(;it!=group->users_list.end();++it)
      {
        if(uids)std::cout<<(*it)->uid<<std::endl;
        else std::cout<<(*it)->username<<std::endl;       
      }
    }
  }
}

void sop::users::Module::cH_groupchange(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h") || params.size()!=3)
  {
    std::cout<<"groupchange [-h] username gid"<<std::endl;
    std::cout<<"Changes group that user belongs to"<<std::endl;
    return;
  }
  else
  {
    if(!_permissions_manager.isSuperUser(sop::process::getProcess(0)))
    {
      std::cout<<"Not a super user."<<std::endl;
      return;
    }
    boost::shared_ptr<User> user = _users_manager.findUser(params[params.size()-2]);
    if(!user)
    {
      std::cout<<"User not found"<<std::endl;
      return;
    }
    boost::shared_ptr<Group> new_group = _groups_manager.findGroup(params[params.size()-1]);
    if(!new_group)
    {
      std::cout<<"New group not found"<<std::endl;
      return;
    }

    new_group->users_list.push_back(user);

    boost::shared_ptr<Group> old_group = _groups_manager.findGroup(user->gid);
    if(old_group)
    {
      old_group->users_list.remove(user);
    }

    user->gid=new_group->gid;
  }
}

void sop::users::Module::cH_nice(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"nice [-h] [-s] gid priority"<<std::endl;
    std::cout<<"Shows nice priority of group/user. -s saves current nice settings"<<std::endl;
    return;
  }

  if(sop::system::Shell::hasParam(params,"-s"))
  {
    if(!_permissions_manager.isSuperUser(sop::process::getProcess(0)))
    {
      std::cout<<"Not a super user."<<std::endl;
      return;
    }
    //ToDo: save file
    return;
  }

  if(params.size()!=3)return;

  gid_t gid = sop::StringConverter::convertStringTo<gid_t>(params[params.size()-2]);
  priority_t priority = (priority_t)sop::StringConverter::convertStringTo<int16_t>(params[params.size()-1]);

  if(priority<kDefault_priority)
  {
    if(!_permissions_manager.isSuperUser(sop::process::getProcess(0)))
    {
      std::cout<<"Not a super user."<<std::endl;
      return;
    }
  }

  _priority_manager.setGroupPriority(gid,priority);

  if(sop::system::Shell::hasParam(params,"-s"))
  {
    //ToDo: save file
  }
}

void sop::users::Module::cH_shownice(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"shownice [-g gid] [-u uid] [-h]"<<std::endl;
    std::cout<<"Shows nice priority of group/user"<<std::endl;
  }
  else if(sop::system::Shell::hasParam(params,"-g"))
  {
    std::cout<<(int16_t)_priority_manager.getGroupPriority(sop::StringConverter::convertStringTo<gid_t>(sop::system::Shell::getParamValue(params,"-g")))<<std::endl;
  }
  else if(sop::system::Shell::hasParam(params,"-u"))
  {
    std::cout<<(int16_t)_priority_manager.getUserPriority(sop::StringConverter::convertStringTo<uid_t>(sop::system::Shell::getParamValue(params,"-u")))<<std::endl;
  }
}

void sop::users::Module::cH_removeniceentry(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"removeniceentry [-h] gid"<<std::endl;
    std::cout<<"Removes priority entry"<<std::endl;
    return;
  }
  _priority_manager.removeGroupPriorityEntry(sop::StringConverter::convertStringTo<gid_t>(params[params.size()-1]));
}

void sop::users::Module::cH_whois(const std::vector<const std::string> & params)
{
  if(params.size()>1)
  {
    std::cout<<"whois [-h]"<<std::endl;
    std::cout<<"Prints username of currently logged user."<<std::endl;
    return;
  }
  boost::shared_ptr<sop::process::Process> shell_process = sop::process::getProcess(0);
  if(!shell_process)
  {
    std::cout<<"No shell process?!"<<std::endl;
    return;
  }
  boost::shared_ptr<User> user = _users_manager.findUser(shell_process->uid);
  if(!user)
  {
    std::cout<<"User not found."<<std::endl;
  }
  else
  {
    std::cout<<user->username<<std::endl;
  }
}

void sop::users::Module::cH_login(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h") || (params.size()!=2 && params.size()!=4))
  {
    std::cout<<"login [-h] username [-p password]"<<std::endl;
    std::cout<<"Logins on user specified by username."<<std::endl;
    return;
  }
  boost::shared_ptr<sop::process::Process> shell_process = sop::process::getProcess(0);
  std::string password="";
  if(sop::system::Shell::hasParam(params,"-p"))password=sop::system::Shell::getParamValue(params,"-p");
  if(!_users_manager.login(shell_process,params[1],password))
  {
    std::cout<<"Logging  failed"<<std::endl;
  }
  else
  {
    std::cout<<"Logged on"<<std::endl;
  }
}

void sop::users::Module::cH_access(const std::vector<const std::string> & params)
{
    if(sop::system::Shell::hasParam(params,"-h") || params.size()!=3)
  {
    std::cout<<"access [-h] filename mode"<<std::endl;
    std::cout<<"Shows if current user has access to file/directory."<<std::endl;
    return;
  }
  sop::files::File *file= _kernel->getFilesModule()->fsxxxx->openFile(sop::process::getProcess(0),getPathFromParam(params[1]),"r");
  if(!file)
  {
    std::cout<<"Access problem."<<std::endl;
  }
  sop::files::Inode *file_inode = file->getInode();
  boost::shared_ptr<sop::process::Process> shell_process = sop::process::getProcess(0);
  if(_permissions_manager.hasPermission(file_inode,shell_process,PermissionsUtilities::getFromRWXString(params[2])))
  {
    std::cout<<"You have permissions to that file."<<std::endl;
  }
  else
  {
    std::cout<<"You dont have permissions to that file."<<std::endl;
  }
  _kernel->getFilesModule()->fsxxxx->closeFile(file);
}

void sop::users::Module::cH_chmod(const std::vector<const std::string> & params)
{
    if(sop::system::Shell::hasParam(params,"-h") || params.size()!=5)
  {
    std::cout<<"chmod [-h] filename user_mode group_mode others_mode"<<std::endl;
    std::cout<<"Changes permissions to a file."<<std::endl;
    return;
  }
  sop::files::File *file= _kernel->getFilesModule()->fsxxxx->openFile(sop::process::getProcess(0),getPathFromParam(params[1]),"w");
  if(!file)
  {
    std::cout<<"Access problem."<<std::endl;
  }
  sop::files::Inode *file_inode = file->getInode();
  boost::shared_ptr<sop::process::Process> shell_process = sop::process::getProcess(0);
  Permissions permissions;
  permissions.user=PermissionsUtilities::getFromRWXString(params[2]);
  permissions.group=PermissionsUtilities::getFromRWXString(params[3]);
  permissions.others=PermissionsUtilities::getFromRWXString(params[4]);
  if(_permissions_manager.changePermissions(file_inode,shell_process,permissions))
  {
    std::cout<<"Mods changed."<<std::endl;
  }
  else
  {
    std::cout<<"Mods not changed."<<std::endl;
  }
  _kernel->getFilesModule()->fsxxxx->closeFile(file);
}

void sop::users::Module::cH_chown(const std::vector<const std::string> & params)
{
    if(sop::system::Shell::hasParam(params,"-h") || params.size()!=3)
  {
    std::cout<<"chown [-h] filename uid"<<std::endl;
    std::cout<<"Changes file owner."<<std::endl;
    return;
  }
  sop::files::File *file= _kernel->getFilesModule()->fsxxxx->openFile(sop::process::getProcess(0),getPathFromParam(params[1]),"w");
  if(!file)
  {
    std::cout<<"Access problem."<<std::endl;
  }
  sop::files::Inode *file_inode = file->getInode();
  boost::shared_ptr<sop::process::Process> shell_process = sop::process::getProcess(0);
  if(_permissions_manager.changeOwner(file_inode,shell_process,StringConverter::convertStringTo<uid_t>(params[2])))
  {
    std::cout<<"Owner changed."<<std::endl;
  }
  else
  {
    std::cout<<"Owner not changed."<<std::endl;
  }
  _kernel->getFilesModule()->fsxxxx->closeFile(file);
}

void sop::users::Module::cH_chgrp(const std::vector<const std::string> & params)
{
    if(sop::system::Shell::hasParam(params,"-h") || params.size()!=3)
  {
    std::cout<<"chgrp [-h] filename gid"<<std::endl;
    std::cout<<"Changes filename group owner."<<std::endl;
    return;
  }
  sop::files::File *file= _kernel->getFilesModule()->fsxxxx->openFile(sop::process::getProcess(0),getPathFromParam(params[1]),"w");
  if(!file)
  {
    std::cout<<"Access problem."<<std::endl;
  }
  sop::files::Inode *file_inode = file->getInode();
  boost::shared_ptr<sop::process::Process> shell_process = sop::process::getProcess(0);
  if(_permissions_manager.changeGroup(file_inode,shell_process,StringConverter::convertStringTo<gid_t>(params[2])))
  {
    std::cout<<"Group changed."<<std::endl;
  }
  else
  {
    std::cout<<"Group not changed."<<std::endl;
  }
  _kernel->getFilesModule()->fsxxxx->closeFile(file);
}