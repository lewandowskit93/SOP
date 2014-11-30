#ifndef _SOP_PROCESSOR_PROCESSOR_H_
#define _SOP_PROCESSOR_PROCESSOR_H_
#include <stdint.h>
#include <stack>

namespace sop
{

  namespace processor
  {

    struct processor
    {
      uint16_t a,b,c,d; // 16bit registers of the processor, a , b , c , d
      uint16_t cs; // code segment, hold where code begins in memory
      uint16_t ip; // instruction pointer, it's more like instruction counter, it counts the instructions
      short quant_time; // the number of instructions readed at one time from memory
      std::stack <uint16_t> processor_stack; // it's a buffor for memory
    };

  }

}

#endif