#ifndef _SOP_SYSTEM_SYSTEM_H_
#define _SOP_SYSTEM_SYSTEM_H_

#include <string>
#include <vector>
#include <boost\shared_ptr.hpp>
#include ".\sop\object.h"
#include ".\sop\system\shell.h"
#include ".\sop\logger\logger.h"
#include ".\sop\users\module.h"
#include ".\sop\files\module.h"
#include ".\sop\processes\module.h"
#include ".\sop\memory\module.h"
#include ".\sop\processor\module.h"

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
        enum State{
          ERROR,
          PRE_INIT,
          INITIALIZED,
          RUNNING,
          SHUTTING_DOWN
        };

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
        void run();

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

        sop::users::Module * getUsersModule();
        const sop::users::Module * getUsersModule() const;

        sop::files::Module * getFilesModule();
        const sop::files::Module * getFilesModule() const;

        sop::processes::Module * getProcessesModule();
        const sop::processes::Module * getProcessesModule() const;

        sop::memory::Module * getMemoryModule();
        const sop::memory::Module * getMemoryModule() const;

        sop::processor::Module * getProcessorModule();
        const sop::processor::Module * getProcessorModule() const;

        std::vector<sop::system::Module*> getModules();
        std::vector<const sop::system::Module*> getModules() const;

        /*
          Sets system state to shutting down.
        */
        void shutdown() const;

        State getSystemState() const;

        /*
          Returns true if server is shutting down.
          Returns false otherwise.
        */
        bool isShuttingDown() const;

      protected:
        
      private:
        mutable State _system_state;
        boost::shared_ptr<sop::logger::Logger> _logger;
        sop::system::Shell _shell;
        sop::users::Module _users_module;
        sop::files::Module _files_module;
        sop::processes::Module _processes_module;
        sop::memory::Module _memory_module;
        sop::processor::Module _processor_module;
        std::vector<sop::system::Module*> _modules;

    };
  }
}

#endif