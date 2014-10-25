#ifndef _SOP_SYSTEM_SHELL_H_
#define _SOP_SYSTEM_SHELL_H_

#include <string>
#include ".\sop\object.h"

namespace sop
{
  namespace system
  {
    /*
      Shell is used to manage shell commands.
    */
    class Shell : public sop::Object
    {
      public:
        Shell();
        virtual ~Shell();
        virtual std::string getClassName() const;

      protected:

      private:
    };
  }
}

#endif