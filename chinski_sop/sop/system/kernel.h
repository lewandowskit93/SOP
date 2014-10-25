#ifndef _SOP_SYSTEM_KERNEL_H_
#define _SOP_SYSTEM_KERNEL_H_

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
#include ".\sop\initializable.h"

namespace sop
{
  namespace system
  {
    
    /*
      Kernel class.
      Consists of different modules (e.g. processor, memory).
      Manages and initializes modules, shell and logger.
    */
    class Kernel :
      public sop::Object,
      public sop::Initializable
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
          Creates kernel with default logging level.
        */
        Kernel();
        /*
          Creates kernel with given logging level.
        */
        explicit Kernel(uint16_t logging_level);
        virtual ~Kernel();
        virtual std::string getClassName() const;

        /*
          Initializes kernel and its modules.
        */
        virtual void initialize();

        /*
          Runs kernel's loop
        */
        void run();

        /*
          Returns pointer to const kernel's logger.
        */
        const sop::logger::Logger * getLogger() const;

        /*
          Returns pointer to kernel's logger.
        */
        sop::logger::Logger * getLogger();

        /*
          Returns pointer to kernel's shell.
        */
        const sop::system::Shell * getShell() const;
        
        /*
          Returns pointer to kernel's shell.
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
          Sets kernel's state to shutting down.
        */
        void shutdown() const;

        State getKernelState() const;

        /*
          Returns true if kernel is shutting down.
          Returns false otherwise.
        */
        bool isShuttingDown() const;

      protected:
        
      private:
        mutable State _kernel_state;
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