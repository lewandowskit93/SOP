#ifndef _SOP_PROCESSOR_EXECUTIVE_UNIT_H_
#define _SOP_PROCESSOR_EXECUTIVE_UNIT_H_
#include "./sop/processor/processor.h"
#include "./sop/processor/interpreter.h"
#include "./sop/processor/scheduler.h"
#include "./sop/logger/logger.h"
#include <vector>
#include <string>
namespace sop
{
  namespace processor
  {

    class ExecutiveUnit
    {
    public:
      short getQuantTimeLeft(); // get actual quant time that is left 
      void resetQuantTime(); // resets quant time to standard quant time
      std::string processorTick(); // processor ticks once every quant time 
      void activateProcessor(); // initial start of executive unit work
      ExecutiveUnit(sop::logger::Logger *logger); // constructor
      ~ExecutiveUnit(); // deconstructor
      void mainExecutiveLoop();
      sop::logger::Logger* logger;
      sop::interpreter::InterpreterHandler interpreter;
      sop::processor::Scheduler scheduler;
      boost::shared_ptr<sop::process::Process> _runningProcess;
      boost::shared_ptr<sop::process::Process> _lastUsedProcess;

      void cH_showQuantTimeLeft (const std::vector<const std::string> & params);
      void cH_showActiveTaskQueue (const std::vector<const std::string> & params);
      void cH_showUnactiveTaskQueue (const std::vector<const std::string> & params);
      void cH_fullTick(const std::vector<const std::string> & params);
      void cH_interpretLine(const std::vector<const std::string> & params);
      void cH_showActualProcessorState(const std::vector<const std::string> & params);
      void cH_showAnyProcessorState(const std::vector<const std::string> & params);
      
      void testerMethod();
    protected:
    private:
      short _quantTimeLeft; // if == 0 then other process will be executed
      short _standardQuantTime; // the standard quant time (it's starting quant time)
    };

  }
}
#endif