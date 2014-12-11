#ifndef _SOP_PROCESSOR_PROCESSOR_H_
#define _SOP_PROCESSOR_PROCESSOR_H_
#include <boost\shared_ptr.hpp>
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
      //REB
      static void readOneByteFromInputAndSavesItOnYoungestByte(processor *proc); // reads one char from input and saves it on youngest byte of register A
      //WRB
      static void printsOutYoungestByte(processor *proc); // prints youngest byte of register A
      //WRC
      static void printsOutYoungestByteAsASCII(processor *proc); // prints youngest byte of register A as ASCII
      //WRI
      static void printsOutRegisterWithoutSign(processor *proc); // prints uint16_t on output
      //WRU
      static void printsOutRegisterWithSign(processor *proc); // prints int16_t
      //SWB
      static void swapBytes(processor *proc, char reg); // swapping bytes of selecte processor//SWB
      
      //General functions 
      //CAL
      static void savesReturnAdresOnStack(processor *proc); // sets IP on stack, and sets offset (from D reg)
      //RET
      static void loadsValueFromStackAndJumpOnIt(processor *proc); // pops val from stack on do jmp on it
      //REB
      static void saveOnYoungestByte(processor *proc, uint16_t input); // saves on the youngest byte of processor A
      //MUL
      static void multipliesAandB(processor *proc); // A  *  B, and the result is held in C
      //DIV
      static void dividesAandB(processor *proc); // A / B, result in C, rest from division in D

      //JMP
      static void doJMP(processor *proc); // sets offset to given offset value
      //JIZ
      static void doJIZ(processor *proc); // sets offset to given offset value if flag zero is active
      //JNZ
      static void doJNZ(processor *proc); // sets offset to given offset value if flag zero is not active
      //JIA
      static void doJIA(processor *proc); // sets offset to given offset value if first number is gr8r than second
      //JAE
      static void doJAE(processor *proc); // sets offset to given offset value if first number is gr8r or equal to second
      //JIB
      static void doJIB(processor *proc); // sets offset to given offset value if first nubmer is lower than second
      //JBE
      static void doJBE(processor *proc); // sets offset to given offset value if first nubmer is lower or equals to second

      static void printOutProcessorState(processor *proc); // prints out all processor fields
      static void clearProcessor(processor *proc); //sets default(starting) values
      static uint16_t softCharRegisterHandler(processor *proc, char processor_register); // gets processor register value
      static uint16_t* charRegisterHandler(processor *proc, char processor_register); // convert char into variable and sets a pointer on register

      //Command register
      //INC
      static void registerIncrement(processor *proc, char processor_register); // incrementing register field by one
      //DEC
      static void registerDecrement(processor *proc, char processor_register); // decrementing register field by one

      //Command register,value 
      //ADV
      static void registerIncrementByValue(processor *proc, char processor_register, uint16_t value); // incrementing register field by value 
      //SUV
      static void registerDecrementByValue(processor *proc, char processor_register, uint16_t value); // decrementing register field by value
      //MOV
      static void setRegisterField(processor *proc, char processor_register, uint16_t value); // setting value into register field
      
      //Command register,register
      //CMP
      static void compareRegisters(processor *proc, char register_one, char register_two); // comparing two registers, setting proper flags
      //MOR
      static void copySourceRegisterToDestinationRegister(processor *proc, char source_processor_register, char destination_processor_register); // source means processor to copy from,
                                                                                                                                  // and destination means processor to be pasted in
      //ADD
      static void addSourceRegisterToDestinationRegister(processor *proc, char source_processor_register, char destination_processor_register); // e.g destA = destA + srcB  
      //SUB
      static void subtractDestinationRegisterBySourceRegister(processor *proc, char source_processor_register, char destination_processor_register); // e.g destA = destA - srcB

      //Instruction pointer functions
      static void incrementInstructionPointer(processor *proc); // instruction pointer ++
      static void setInstructionPointer(processor *proc, short value); // setting instruction pointer
      static void increasInstructionPointerBy(processor *proc, short value); // increasing instruction pointer by value
     
      //Flags
      static void setZeroFlag(processor *proc); // setting zero flag to true
      static void unsetZeroFlag(processor *proc); // setting zero flag to false
      static void setSignFlag(processor *proc); // setting sign flag to true
      static void unsetSignFlag(processor *proc); // setting sign flag to false

      //Stack
      //PUS
      static void addRegisterToStack(processor *proc, char register); // adds register value to stack
      //POP
      static void popFromStack(processor *proc, char register); // pops from stack to specified register 
      static void clearStack(processor *proc); // cleares all data in the stack

      //bit operations
      //OR
      static void bitwiseOR(processor *proc, char register_one, char register_two); // bit sum on two registers, result will be in reg_one
      //AND
      static void bitwiseAND(processor *proc, char register_one, char register_two); // bit product on two registers
      //NEG
      static void bitNEG(processor *proc, char reg); // will negate bits 
    };
  }

}

#endif