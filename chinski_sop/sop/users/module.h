#ifndef _SOP_USERS_MODULE_H_
#define _SOP_USERS_MODULE_H_

#include <string>
#include <vector>
#include <sstream>
#include ".\sop\system\module.h"
#include ".\sop\users\users_manager.h"
#include ".\sop\users\groups_manager.h"

namespace sop
{
  namespace users
  {
    /*
      Users module class.
    */

    class Module : public sop::system::Module
    {
      public:
        explicit Module(sop::system::Kernel *kernel);
        virtual ~Module();
        virtual std::string getClassName() const;
        virtual void initialize();

        UsersManager* getUsersManager();
        GroupsManager* getGroupsManager();

        void cH_useradd(const std::vector<const std::string> & params);
        void cH_userfind(const std::vector<const std::string> & params);
        void cH_userdel(const std::vector<const std::string> & params);
        void cH_userslist(const std::vector<const std::string> & params);
        void cH_chpasswd(const std::vector<const std::string> & params);

        void cH_groupadd(const std::vector<const std::string> & params);
        void cH_groupfind(const std::vector<const std::string> & params);
        void cH_groupdel(const std::vector<const std::string> & params);
        void cH_groupslist(const std::vector<const std::string> & params);
        void cH_groupmembers(const std::vector<const std::string> & params);
        void cH_groupchange(const std::vector<const std::string> & params);
        

      protected:

      private:
        UsersManager _users_manager;
        GroupsManager _groups_manager;
    };
  }
}

#endif