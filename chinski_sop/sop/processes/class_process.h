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
      void setArrayPages(uint16_t);
      //getters
      uint16_t getPID();
      uint16_t getUID();
      uint16_t getGID();
      uint16_t getArrayPages();
      //enumeration
      enum TASK_STATUS
      {
        task_new = 1,       //nowy proces, ktory czeka na przydzial pamieci
        task_waiting = 2,   //proces, ktory otrzymal pamiec i czeka na procesor
        task_running = 3,   //proces, ktory otrzymal obydwa zasoby i rozpoczal prace
        task_executed = 4   //proces, ktory zakonczyl swoje dzialanie
      };
      //variables
      uint16_t _PID, _UID, _GID;
      uint16_t _register_A, _register_B, _register_C, _register_D;
      uint16_t _stack_counter;
      uint16_t _array_pages;

    private:
      void sendToScheduler(); //funkcja wysylajaca do planisty info, ze proces moze byc brany pod uwage(tymczasowo typu void)
      //set enums
      uint16_t setStatusNew();
      uint16_t setStatusWaiting();
      uint16_t setStatusRunning();
      uint16_t setStatusExecuted();
      //setters
      void setPPID(uint16_t);
      void setMemoryFlagStatus(uint16_t);
      void setProcessorFlagStatus(uint16_t);
      //getters
      uint16_t getPPID();
      uint16_t getMemoryFlagStatus();
      uint16_t getProcessorFlagStatus();
      //methods

      //variables
      uint16_t _PPID;
      uint16_t _memoryFlagStatus;     // flaga ustawiana na 1 jesli zostanie przydzielona pamiec
      uint16_t _processorFlagStatus;  // flaga ustawiana na 1 jesli zostanie przydzielony procesor

    };
  }
}

#endif