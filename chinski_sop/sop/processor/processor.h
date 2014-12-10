#ifndef _SOP_PROCESSOR_PROCESSOR_H_
#define _SOP_PROCESSOR_PROCESSOR_H_
#include "./sop/string_converter.h"
#include <stdint.h>
#include <stack>
#include <iomanip>
#include <string>
#include <iostream>

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
      uint16_t zero_flag; // if (false) then operation result is not equal to 0, otherwise is equal.
      uint16_t sign_flag; // if (false) then operation result is positive (>=0), otherwise is negative (<0)
    
      processor(); // constructor
      ~processor(); // deconstructor
    };


    class ProcessorHandler // this class will handle any operation done on the processor fields
    {

    public:

      //Input/Output
      static void readOneByteFromInputAndSavesItOnYoungestByte(processor *proc); // reads one char from input and saves it on youngest byte of register A
      static void printsOutYoungestByte(processor *proc); // prints youngest byte of register A
      static void printsOutYoungestByteAsASCII(processor *proc); // prints youngest byte of register A as ASCII
      static void printsOutRegisterWithoutSign(processor *proc); // prints uint16_t on output
      static void printsOutRegisterWithSign(processor *proc); // prints int16_t
      //General functions 
      static void saveOnYoungestByte(processor *proc, uint16_t input); // saves on the youngest byte of processor A

      static void multipliesAandB(processor *proc); // A  *  B, and the result is held in C
      static void dividesAandB(processor *proc); // A / B, result in C, rest from division in D

      static void doJMP(processor *proc, uint16_t offset); // sets offset to given offset value
      // I decided first number means register A and second B
      static void doJIZ(processor *proc, uint16_t offset); // sets offset to given offset value if flag zero is active
      static void doJNZ(processor *proc, uint16_t offset); // sets offset to given offset value if flag zero is not active
      static void doJIA(processor *proc, uint16_t offset); // sets offset to given offset value if first number is gr8r than second
      static void doJAE(processor *proc, uint16_t offset); // sets offset to given offset value if first number is gr8r or equal to second
      static void doJIB(processor *proc, uint16_t offset); // sets offset to given offset value if first nubmer is lower than second
      static void doJBE(processor *proc, uint16_t offset); // sets offset to given offset value if first nubmer is lower or equals to second

      static void printOutProcessorState(processor *proc); // prints out all processor fields
      static void clearProcessor(processor *proc); //sets default(starting) values
      static uint16_t softCharRegisterHandler(processor *proc, char processor_register); // gets processor register value
      static uint16_t* charRegisterHandler(processor *proc, char processor_register); // convert char into variable and sets a pointer on register

      //Command register
      static void registerIncrement(processor *proc, char processor_register); // incrementing register field by one
      static void registerDecrement(processor *proc, char processor_register); // decrementing register field by one

      //Command register,value 
      
      static void registerIncrementByValue(processor *proc, char processor_register, uint16_t value); // incrementing register field by value 
      static void registerDecrementByValue(processor *proc, char processor_register, uint16_t value); // decrementing register field by value
      static void setRegisterField(processor *proc, char processor_register, uint16_t value); // setting value into register field
      
      //Command register,register
      static void compareRegisters(processor *proc, char register_one, char register_two); // comparing two registers, setting proper flags
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
      static void addValueToStack(processor *proc, uint16_t value); // adds value to stack
      static void addRegisterToStack(processor *proc, char register); // adds register value to stack
      static void popFromStack(processor *proc, char register); // pops from stack to specified register 
      static void clearStack(processor *proc); // cleares all data in the stack

      //bit operations
      static void bitwiseOR(processor *proc, char register_one, char register_two); // bit sum on two registers, result will be in reg_one
      static void bitwiseAND(processor *proc, char register_one, char register_two); // bit product on two registers
      static void bitNEG(processor *proc, char reg); // will negate bits 
    };
  }

}

#endif