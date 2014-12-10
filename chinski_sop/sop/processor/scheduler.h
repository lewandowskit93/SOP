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
      boost::shared_ptr<sop::process::Process> getHighestPriorityProcess(); // takes the highest priority process to execute
      static void addProcess(boost::shared_ptr<sop::process::Process> p, sop::processor::Scheduler *sched);
      static void removeProcess(boost::shared_ptr<sop::process::Process> p, sop::processor::Scheduler *sched);
      uint8_t getUserPriority(boost::shared_ptr<sop::process::Process> p); // from Tomeks layer i'll load nice parameter
      void calculatePriority(); // will calculate priorities for all tasks in both vectors
      void addToUnactiveTaskArray(boost::shared_ptr<sop::process::Process> p); // adding to unactive task array
      void addToActiveTaskArray(boost::shared_ptr<sop::process::Process> p); // adding to active task array
      void eraChange(); // will change unactive one to active 
      bool isEraChangeNeeded(); // checks if era change has to be done
      bool firstIsActive(); // checks if the first array is the active one
      bool secondIsActive(); // checks if the second array is the active one
      void clearTaskArray(); // used to clean vector after eraChange
      void printOutActiveTasks();
      void printOutUnactiveTasks();
      void printOutHelperMethod(int i, bool which);
      Scheduler();
      ~Scheduler();
    protected:
    private:

      std::vector <std::queue<boost::shared_ptr<sop::process::Process> >> _first_task_array; // the first array of tasks [active/unactive], short is the nice parameter
      bool _isFirstActive; // tells if the first array is active
      std::vector <std::queue<boost::shared_ptr<sop::process::Process> >>  _second_task_array; // the second array of tasks [unactive/active], short is the nice parameter
      // short comment: vector of queues will be used like: vector[0] = -4, vector[1] = -3
      // other words.. nice + 5 (0-9)
      bool _isSecondActive; // tells if the second array is active
    };

  }

}

#endif