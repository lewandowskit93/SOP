#ifndef _SOP_SYSTEM_MODULE_H_
#define _SOP_SYSTEM_MODULE_H_

#include <string>
#include ".\sop\object.h"

namespace sop
{
  namespace system
  {

    class System;

    /*
      Module abstract class.
      Every module should inherit from it.
      Module is component of the system.
    */
    class Module : public sop::Object
    {
      public:
        /*
          Constructor for Module element.
          Params:
              system - pointer to system that module is creating for.
              Pointer should be valid for whole module lifetime.
        */
        explicit Module(sop::system::System *system);

        virtual ~Module();

        virtual std::string getClassName() const;

        /*
          Should initialize everything that module needs to work.
          E.g. register functions to shell.

          Throws sop::system::exceptions::ModuleInitializationException on failure.
        */
        virtual void initializeModule() = 0;
      protected:
        /*
          Pointer to system, that module is contained in.
        */
        sop::system::System *_system;

      private:

    };    
  }
}

#endif