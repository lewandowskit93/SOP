#ifndef _SOP_PROCESS_PROCESS_H_
#define _SOP_PROCESS_PROCESS_H_
#include "./sop/processor/processor.h"
namespace sop
{
  namespace process
  {
    class Process
    {
    public:
      Process(){
       PID = rand()%4;
     }

      uint16_t PID;
      sop::processor::processor procek;
      
   };
  }
}
#endif