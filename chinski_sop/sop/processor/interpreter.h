#ifndef _SOP_PROCESSOR_INTERPRETER_H_
#define _SOP_PROCESSOR_INTERPRETER_H_

#include "./sop/processor/processor.h"
#include "./sop/processes/class_process.h"
#include "./sop/string_converter.h"
#include "./sop/logger/logger.h"
#include <boost\smart_ptr.hpp>
#include <string.h>
#include <iostream>
namespace sop
{
  namespace interpreter
  {
    class InterpreterHandler // will handle bytes received from memory and store program information
    {
    public:
      char getByteFromMemory(boost::shared_ptr<sop::process::Process> p); // asking for 1 byte from memory
      void buildProgramLine(boost::shared_ptr<sop::process::Process> p); // will build one line e.g. 'ADD A,B\n'
      std::string interpretLine(boost::shared_ptr<sop::process::Process> p); // will interpret one line
      void pickDataPart(std::string s);
      void pickCommandPart(std::string s);

      void printDataPart();
      void printCommandPart();

      void interpreterReset();

      std::string getDataPart();
      std::string getCommandPart();

      InterpreterHandler(sop::logger::Logger* logger);
      sop::logger::Logger* logger;
    protected:
    private:
      std::string _command_part; 
      std::string _data_part;
      std::string _program_line; // the one line of program code
    };
  }
}
#endif