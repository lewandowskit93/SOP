#ifndef _SOP_PROCESSOR_INTERPRETER_H_
#define _SOP_PROCESSOR_INTERPRETER_H_

#include "./sop/processor/processor.h"
#include <boost\smart_ptr.hpp>
#include <string.h>
namespace sop
{
  namespace interpreter
  {
    class InterpreterHandler // will handle bytes received from memory and store program information
    {
    public:
      char getByteFromMemory(sop::processor::processor *processor); // asking for 1 byte from memory
      void buildProgramLine(); // will build one line e.g. 'ADD A,B\n'
      void interpretLine(); // will interpret one line
    protected:
    private:
      std::string program_line; // the one line of program code
    };
  }
}
#endif