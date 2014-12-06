#ifndef _SOP_USERS_USERS_MANAGER_H_
#define _SOP_USERS_USERS_MANAGER_H_

#include <list>
#include <boost\shared_ptr.hpp>
#include ".\sop\users\id_definitions.h"
#include ".\sop\object.h"
#include ".\sop\users\user.h"

//#define PASSWD_FILE_INTEGRATED;

namespace sop
{
  namespace users
  {

    class Module;

    class UsersManager : public Object
    {
      public:
        explicit UsersManager(Module *module);
        virtual ~UsersManager();
        virtual std::string getClassName() const;
        bool addUser(boost::shared_ptr<User> user);
        bool addUser(const User & user);
        bool deleteUser(const std::string & username);
        boost::shared_ptr<User> findUser(uid_t uid);
        boost::shared_ptr<User> findUser(const std::string & username);
        uid_t getNextFreeUID(uid_t greater_than = kMax_system_uid);
        bool isUIDFree(uid_t uid);
        bool isUsernameFree(const std::string & username);
        std::list<boost::shared_ptr<User>> getUsersList();
        void loadUsersFromFile(const std::string & filename);
        void saveUsersToFile(const std::string & filename);

      protected:

        Module *_module;
        std::list<boost::shared_ptr<User>> _users_list;
      private:

    };
  }
}

#endif