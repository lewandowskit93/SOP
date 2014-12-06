#ifndef _SOP_USERS_MODULE_H_
#define _SOP_USERS_MODULE_H_

#include <string>
#include <vector>
#include <sstream>
#include ".\sop\system\module.h"
#include ".\sop\users\users_manager.h"

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

        bool hasParam(const std::vector<const std::string> & param, const std::string & param_name);
        std::string getParamValue(const std::vector<const std::string> & param, const std::string & param_name);

        template<class T>
        T convertStringTo(const std::string & value)
        {
          T conv_value=0;
          if(value!="")
          {
            std::stringstream ss;
            ss<<value;
            ss>>conv_value;
          }
          return conv_value;
        }

        void cH_useradd(const std::vector<const std::string> & params);
        void cH_userfind(const std::vector<const std::string> & params);
        void cH_userdel(const std::vector<const std::string> & params);
        void cH_userslist(const std::vector<const std::string> & params);
        

      protected:

      private:
        UsersManager _users_manager;
    };
  }
}

#endif