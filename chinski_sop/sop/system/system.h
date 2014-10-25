#ifndef _SOP_SYSTEM_SYSTEM_H_
#define _SOP_SYSTEM_SYSTEM_H_

#include <string>
#include ".\sop\object.h"
#include ".\sop\system\shell.h"
#include ".\sop\logger\logger.h"

namespace sop{
  namespace system{
    
    /*
      System class.
      Consists of different modules (e.g. processor, memory).
      Manages and initializes modules, shell and logger.
    */
    class System : public sop::Object{
      public:
        System();
        virtual ~System();
        virtual std::string getClassName();

        /*
          Initializes system and its modules.
        */
        void initializeSystem();

        /*
          Runs system's loop
        */
        void runSystem();

        /*
          Returns pointer to system's logger.
        */
        sop::logger::Logger * getLogger();

        /*
          Returns pointer to system's shell.
        */
        sop::system::Shell * getShell();

      protected:
        
      private:
        sop::logger::Logger _logger;
        sop::system::Shell _shell;
    };
  }
}

#endif