#ifndef _SOP_SYSTEM_SHELL_H_
#define _SOP_SYSTEM_SHELL_H_

#include <string>
#include <map>
#include <vector>
#include <boost\bind.hpp>
#include <boost\signals2.hpp>
#include ".\sop\object.h"
#include ".\sop\exception.h"

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
        typedef void (CommandHandler)(const std::vector<const std::string>&);

        /*
          Command handler function signal signature.
        */
        typedef boost::signals2::signal<CommandHandler> CommandHandlerSignal;

        /*
          Exception that is thrown if shell line is invalid.
        */
        static class ParsingException : public sop::Exception
        {
          public:
            ParsingException();
            explicit ParsingException(const std::string & value);
            virtual ~ParsingException();
            virtual std::string getClassName() const;
          protected:
          private:
        };

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
        bool registerCommand(const std::string & command,void (ClassType::*method)(const std::vector<const std::string>&), ClassType * object)
        {
          return doRegisterCommand(command,boost::bind(method,object,_1));
        }

        /*
          Parses line to vector of arguments.
          First argument is the command, the rest are the parameters.
          If line contains only whitespaces then the vector is empty.
          If any problem is encountered then sop::system::Shell::ParsingException is thrown.
        */
        std::vector<std::string> parse(const std::string & line);

        /*
          Converts vector of strings to vector of constant strings.
        */
        std::vector<const std::string> toVectorOfConst(const std::vector<std::string> & vect);

        /*
          Returns vector of all registered commands in the shell.
        */
        std::vector<std::string> getRegisteredCommands();

        /*
          Returns true when param vector contains element equal to param_name.
          Returns false otherwise.
        */
        static bool hasParam(const std::vector<const std::string> & param, const std::string & param_name);

        /*
          Returns value of parameter specified by param_name.
          Value is the next element after param_name in params vector.
          If there is no parameter named param_name in the vector, or parameter is the last element (doesn't have value)
           then the empty string is returned.
        */
        static std::string getParamValue(const std::vector<const std::string> & param, const std::string & param_name);

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