#ifndef _SOP_PROCESSOR_SCHEDULER_H_
#define _SOP_PROCESSOR_SCHEDULER_H_
#include "./sop/processor/processor.h"
#include "./sop/processor/interpreter.h"
#include "./sop/processes/class_process.h"
#include <vector>
#include <queue>
namespace sop
{

  namespace processor
  {

    class Scheduler
    {
    public:
      sop::process::Process *getHighestPriorityProcess(); // takes the highest priority process to execute
      static void addProcess(sop::process::Process *p, sop::processor::Scheduler *sched);
      uint8_t getUserPriority(sop::process::Process *p); // from Tomeks layer i'll load nice parameter
      void calculatePriority(); // will calculate priorities for all tasks in both vectors
      void addToUnactiveTaskArray(sop::process::Process *p); // adding to unactive task array
      void addToActiveTaskArray(sop::process::Process *p); // adding to active task array
      void eraChange(); // will change unactive one to active 
      bool firstIsActive(); // checks if the first array is the active one
      bool secondIsActive(); // checks if the second array is the active one
      void clearTaskArray(); // used to clean vector after eraChange
      Scheduler();
      ~Scheduler();
    protected:
    private:

      std::vector <std::queue<sop::process::Process*>> _first_task_array; // the first array of tasks [active/unactive], short is the nice parameter
      bool _isFirstActive; // tells if the first array is active
      std::vector <std::queue<sop::process::Process*>>  _second_task_array; // the second array of tasks [unactive/active], short is the nice parameter
      // short comment: vector of queues will be used like: vector[0] = -4, vector[1] = -3
      // other words.. nice + 5 (0-9)
      bool _isSecondActive; // tells if the second array is active
    };

  }

}

#endif