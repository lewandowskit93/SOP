#ifndef _SOP_USERS_MODULE_H_
#define _SOP_USERS_MODULE_H_

#include <string>
#include ".\sop\system\module.h"

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
        explicit Module(sop::system::System *system);
        virtual ~Module();
        virtual std::string getClassName() const;
        virtual void initializeModule();

      protected:

      private:

    };
  }
}

#endif