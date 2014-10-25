#ifndef _SOP_PROCESSES_MODULE_H_
#define _SOP_PROCESSES_MODULE_H_

#include <string>
#include ".\sop\system\module.h"

namespace sop
{
  namespace processes
  {
    /*
      Processes module class.
    */
    class Module : public sop::system::Module
    {
      public:
        explicit Module(sop::system::Kernel *kernel);
        virtual ~Module();
        virtual std::string getClassName() const;
        virtual void initializeModule();

      protected:

      private:

    };
  }
}

#endif