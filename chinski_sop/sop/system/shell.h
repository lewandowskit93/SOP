#ifndef _SOP_SYSTEM_SHELL_H_
#define _SOP_SYSTEM_SHELL_H_

#include <string>
#include <map>
#include <vector>
#include <boost\bind.hpp>
#include <boost\signals2.hpp>
#include ".\sop\object.h"

namespace sop
{
  namespace system
  {
    class Kernel;

    /*
      Shell is used to manage shell commands.
    */
    class Shell : public sop::Object
    {
      public:
        /*
          Command handler function signature.
        */
        typedef std::string (CommandHandler)(const std::vector<const std::string>&);

        /*
          Command handler function signal signature.
        */
        typedef boost::signals2::signal<CommandHandler> CommandHandlerSignal;

        explicit Shell(sop::system::Kernel * kernel);
        virtual ~Shell();
        virtual std::string getClassName() const;
        sop::system::Kernel * getKernel();
        const sop::system::Kernel * getKernel() const;

        /*
          Invokes next shell step.
          Shell step reads one line from command line,
          parses it and executes proper shell command.
        */
        void step();

        /*
          Registers function as a handler for the command.
        */
        bool registerCommand(const std::string & command, CommandHandler * function);

        /*
          Registers class method as a handler for the command.
        */
        template <class ClassType>
        bool registerCommand(const std::string & command,std::string (ClassType::*method)(const std::vector<const std::string>&), ClassType * object)
        {
          return doRegisterCommand(command,boost::bind(method,object,_1));
        }

      protected:

      private:

        /*
          Registers handler for the command.
        */
        template <typename HandlerType>
        bool doRegisterCommand(const std::string & command, HandlerType command_handler)
        {
          _kernel->getLogger()->logShell(sop::logger::Logger::Level::INFO, "Registering shell command: "+command);
          if(_commands.count(command) == 0)
          {
            boost::shared_ptr<CommandHandlerSignal> handler(new CommandHandlerSignal);
            handler->connect(command_handler);
            _commands[command]=handler;
            _kernel->getLogger()->logShell(sop::logger::Logger::Level::INFO, "Command registered.");
            return true;
          }
          else
          {
            _kernel->getLogger()->logShell(sop::logger::Logger::Level::WARNING, "Shell command: "+command+" already registered.");
            return false;
          }
        }

        /*
          Kernel, that shell is runned on.
        */
        sop::system::Kernel * _kernel;

        std::string _last_input_line;

        /*
          Maps commands names to handling functions/methods.
        */
        std::map<std::string, boost::shared_ptr<CommandHandlerSignal>> _commands;
    };
  }
}

#endif