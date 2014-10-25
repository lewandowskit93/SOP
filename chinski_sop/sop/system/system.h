#ifndef _SOP_SYSTEM_SYSTEM_H_
#define _SOP_SYSTEM_SYSTEM_H_

#include <string>
#include <boost\shared_ptr.hpp>
#include ".\sop\object.h"
#include ".\sop\system\shell.h"
#include ".\sop\logger\logger.h"

namespace sop
{
  namespace system
  {
    
    /*
      System class.
      Consists of different modules (e.g. processor, memory).
      Manages and initializes modules, shell and logger.
    */
    class System : public sop::Object
    {
      public:
        /*
          Creates system with default logging level.
        */
        System();
        /*
          Creates system with given logging level.
        */
        explicit System(uint16_t logging_level);
        virtual ~System();
        virtual std::string getClassName() const;

        /*
          Initializes system and its modules.
        */
        void initializeSystem();

        /*
          Runs system's loop
        */
        void runSystem();

        /*
          Returns pointer to const system's logger.
        */
        const sop::logger::Logger * getLogger() const;

        /*
          Returns pointer to system's logger.
        */
        sop::logger::Logger * getLogger();

        /*
          Returns pointer to system's shell.
        */
        const sop::system::Shell * getShell() const;
        
        /*
          Returns pointer to system's shell.
        */
        sop::system::Shell * getShell();




      protected:
        
      private:
        boost::shared_ptr<sop::logger::Logger> _logger;
        sop::system::Shell _shell;
    };
  }
}

#endif