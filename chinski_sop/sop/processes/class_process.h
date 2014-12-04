#ifndef _SOP_PROCESS_PROCESS_H_
#define _SOP_PROCESS_PROCESS_H_

#include <iostream>
#include <string>
#include <cstdint>


namespace sop
{
  namespace process
  {
    class Process
    {
    public:  
  
    protected:
      //setters
      void setPID(uint16_t);
      void setUID(uint16_t);
      void setGID(uint16_t);
      void setRegisterA(uint16_t);
      void setRegisterB(uint16_t);
      void setRegisterC(uint16_t);
      void setRegisterD(uint16_t);
      void setStackCounter(uint16_t);
      void setArrayPages(uint16_t);
      //getters
      uint16_t getPID();
      uint16_t getUID();
      uint16_t getGID();
      uint16_t getRegisterA();
      uint16_t getRegisterB();
      uint16_t getRegisterC();
      uint16_t getRegisterD();
      uint16_t getStackCounter();
      uint16_t getArrayPager();
      //enumeration
      enum TASK_STATUS
      {
        task_new = 1, //nowy proces, który czeka na przydzia³ pamiêci
        task_waiting = 2, //proces, który otrzyma³ pamiêæ i czeka na procesor
        task_running = 3, //proces, który otrzyma³ obywa zasoby i rozpocz¹³ prace
        task_executed = 4 // proces, który zakoñczy³ swoje dzia³anie
      };
      //variables
      int16_t _PID, _UID, _GID;
      int16_t _register_A, _register_B, _register_C, _register_D;
      int16_t _stack_counter;
      int16_t _array_pages;

    private:
      //setters
      void setPPID(uint16_t);
      //getters
      uint16_t getPPID();
      //variables
      int16_t _PPID;

    };
  }
}

#endif