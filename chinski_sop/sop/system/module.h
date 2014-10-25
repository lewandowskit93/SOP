#ifndef _SOP_SYSTEM_MODULE_H_
#define _SOP_SYSTEM_MODULE_H_

#include <string>
#include ".\sop\object.h"
#include ".\sop\initializable.h"

namespace sop
{
  namespace system
  {

    class Kernel;

    /*
      Module abstract class.
      Every module should inherit from it.
      Module is component of the kernel.
    */
    class Module : 
      public sop::Object,
      public sop::Initializable
    {
      public:
        /*
          Constructor for Module element.
          Params:
              kernel - pointer to kernel that module is created for.
              Pointer should be valid for whole module lifetime.
        */
        explicit Module(sop::system::Kernel *kernel);

        virtual ~Module();

        virtual std::string getClassName() const;

        /*
          Should initialize everything that module needs to work.
          E.g. register functions to shell.

          Throws sop::system::exceptions::ModuleInitializationException on failure.
        */
        virtual void initialize() = 0;

        sop::system::Kernel * getKernel();
        const sop::system::Kernel * getKernel() const;
      protected:
        /*
          Pointer to kernel, that module is contained in.
        */
        sop::system::Kernel *_kernel;

      private:

    };    
  }
}

#endif