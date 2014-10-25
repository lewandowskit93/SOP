#ifndef _SOP_SYSTEM_SHELL_H_
#define _SOP_SYSTEM_SHELL_H_

// ToDo: shell commands registration, parsing and execution

#include <string>
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

      protected:

      private:
        /*
          Kernel, that shell is runned on.
        */
        sop::system::Kernel * _kernel;

        std::string _last_input_line;
    };
  }
}

#endif