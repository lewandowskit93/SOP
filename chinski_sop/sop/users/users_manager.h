#ifndef _SOP_USERS_USERS_MANAGER_H_
#define _SOP_USERS_USERS_MANAGER_H_

#include <list>
#include <string>
#include <boost\shared_ptr.hpp>
#include <boost\regex.hpp>
#include ".\sop\users\id_definitions.h"
#include ".\sop\object.h"
#include ".\sop\users\user.h"
#include ".\sop\temporary.h"

#define PASSWD_FILE_INTEGRATED

namespace sop
{
  namespace users
  {

    class Module;
    class Encryptor;

    namespace fakers
    {
      struct pcb;
      struct inode;
    }

    class UsersManager : public Object
    {
      public:
        static const boost::regex username_regex;
        static const boost::regex password_regex;

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
        static bool isUsernameValid(const std::string & username);
        static bool checkPasswordFormat(const std::string & password);
        bool isPasswordValid(boost::shared_ptr<User> user, const std::string & password);
        boost::shared_ptr<Encryptor> getEncryptor();
        bool login(boost::shared_ptr<sop::process::Process> process, const std::string & username, const std::string & password);

      protected:

        Module *_module;
        boost::shared_ptr<Encryptor> _encryptor;
        std::list<boost::shared_ptr<User>> _users_list;
      private:
        
    };
  }
}

#endif