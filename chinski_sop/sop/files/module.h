#ifndef _SOP_FILES_MODULE_H_
#define _SOP_FILES_MODULE_H_

#include <string>
#include ".\sop\system\module.h"
#include ".\sop\files\filesystem.h"

namespace sop
{
  namespace files
  {
    /*
      Files module class.
    */
    class Module : public sop::system::Module
    {
      public:
        explicit Module(sop::system::Kernel *kernel);
        virtual ~Module();
        virtual std::string getClassName() const;
        virtual void initialize();
        Filesystem* fsxxxx;

      protected:

      private:
  
    };
  }
}

#endif