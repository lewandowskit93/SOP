#ifndef _SOP_PROCESSOR_EXECUTIVE_UNIT_H_
#define _SOP_PROCESSOR_EXECUTIVE_UNIT_H_
#include "./sop/processor/processor.h"
#include "./sop/processor/interpreter.h"
#include "./sop/processor/scheduler.h"

namespace sop
{
  namespace processor
  {

    class ExecutiveUnit
    {
    public:
      short getQuantTimeLeft(); // get actual quant time that is left 
      void resetQuantTime(); // resets quant time to standard quant time
      void processorTick(); // processor ticks once every quant time 
      ExecutiveUnit(); // constructor
      ~ExecutiveUnit(); // deconstructor
      sop::interpreter::InterpreterHandler interpreter;
      sop::processor::Scheduler scheduler;
    protected:
    private:
      short _quantTimeLeft; // if == 0 then other process will be executed
      short _standardQuantTime; // the standard quant time (it's starting quant time)
    };

  }
}
#endif