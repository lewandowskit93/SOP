#ifndef _SOP_PROCESSOR_EXECUTIV_EUNIT_H_
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
      void countTicks();
      void processorTick();
    protected:
    private:
      short quantTimeLeft; // if == 0 then other process will be executed
    };

  }
}
#endif