#ifndef _SOP_PROCESSOR_PROCESSOR_H_
#define _SOP_PROCESSOR_PROCESSOR_H_
#include <stdint.h>
#include <stack>
#include <string.h>

namespace sop
{

  namespace processor
  {

    struct processor
    {
      uint16_t a,b,c,d; // 16bit registers of the processor, a , b , c , d
      uint16_t cs; // code segment, hold where code begins in memory
      uint16_t ip; // instruction pointer, it's more like instruction counter, it counts the instructions
      //short quant_time; // the number of instructions readed at one time from memory // UPDATE: it'll be done in executive_unit
      std::stack <uint16_t> processor_stack; // it's a buffor for memory
      bool zero_flag; // if (false) then operation result is not equal to 0, otherwise is equal.
      bool sign_flag; // if (false) then operation result is positive (>=0), otherwise is negative (<0)
    
      processor(); // constructor
      ~processor(); // deconstructor
    };


    class ProcessorHandler // this class will handle any operation done on the processor fields
    {

    public:
      //General functions 
      static void printOutProcessorState(processor *proc);
      static void clearProcessor(processor *proc); //sets default(starting) values
      static uint16_t* charRegisterHandler(processor *proc, char processor_register); // convert chat into variable

      //Command register
      static void registerIncrement(processor *proc, char processor_register); // incrementing register field by one
      static void registerDecrement(processor *proc, char processor_register); // decrementing register field by one

      //Command register,value 
      static void registerIncrementByValue(processor *proc, char processor_register, uint16_t value); // incrementing register field by value 
      static void registerDecrementByValue(processor *proc, char processor_register, uint16_t value); // decrementing register field by value
      static void setRegisterField(processor *proc, char processor_register, uint16_t value); // setting value into register field
      
      //Command register,register
      static void copySourceRegisterToDestinationRegister(processor *proc, char source_processor_register, char destination_processor_register); // source means processor to copy from,
                                                                                                                                  // and destination means processor to be pasted in
      static void addSourceRegisterToDestinationRegister(processor *proc, char source_processor_register, char destination_processor_register); // e.g destA = destA + srcB 
      static void subtractDestinationRegisterBySourceRegister(processor *proc, char source_processor_register, char destination_processor_register); // e.g destA = destA - srcB

      //Instruction pointer functions
      static void incrementInstructionPointer(processor *proc); // instruction pointer ++
      static void setInstructionPointer(processor *proc, short value); // setting instruction pointer
      static void increasInstructionPointerBy(processor *proc, short value); // increasing instruction pointer by value

      //Code segment
      static void loadWhereCodeSegmentStarts(/* process *p */ processor *proc); // loads where code begins in memory

      //Flags
      static void setZeroFlag(processor *proc); // setting zero flag to true
      static void unsetZeroFlag(processor *proc); // setting zero flag to false
      static void setSignFlag(processor *proc); // setting sign flag to true
      static void unsetSignFlag(processor *proc); // setting sign flag to false

      //Stack
      static void addToStack(/**/);
      static void popFromStack(/**/);
      static void clearStack(processor *proc); // cleares all data in the stack


    };
  }

}

#endif