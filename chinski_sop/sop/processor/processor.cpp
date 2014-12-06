#include "./sop/processor/processor.h"

sop::processor::processor::processor():
        a(0), 
        b(0), 
        c(0), 
        d(0), 
        ip(0){}

sop::processor::processor::~processor()
{
}

void sop::processor::ProcessorHandler::clearProcessor(sop::processor::processor *proc)
{
  proc->a = 0;
  proc->b = 0;
  proc->c = 0;
  proc->d = 0;
  proc->cs = 0;
  proc->ip = 0;
  ProcessorHandler::clearStack(proc);
  //proc->quant_time = 3;//Moved to executive_unit
  proc->sign_flag = 0;
  proc->zero_flag = 0;
}

uint16_t* sop::processor::ProcessorHandler::charRegisterHandler(sop::processor::processor *proc, char processor_register)
{
  if (processor_register == 'a' || processor_register == 'A')
  {
    return &proc->a;
  }
  else if (processor_register == 'b' || processor_register == 'B')
  {
    return &proc->b;
  }
  else if (processor_register == 'c' || processor_register == 'C')
  {
    return &proc->c;
  }
  else if (processor_register == 'd' || processor_register == 'D')
  {
    return &proc->d;
  }
  else throw "Bad register";
}
void sop::processor::ProcessorHandler::registerIncrement(sop::processor::processor *proc, char processor_register)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  ++(*reg);
}

void sop::processor::ProcessorHandler::registerDecrement(sop::processor::processor *proc, char processor_register)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  --(*reg);
}

void sop::processor::ProcessorHandler::registerIncrementByValue(sop::processor::processor *proc, char processor_register, uint16_t value)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  (*reg) += value;
}

void sop::processor::ProcessorHandler::registerDecrementByValue(sop::processor::processor *proc, char processor_register, uint16_t value)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  (*reg) -= value;
}

void sop::processor::ProcessorHandler::setRegisterField(sop::processor::processor *proc, char processor_register, uint16_t value)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  (*reg) = value;
}

void sop::processor::ProcessorHandler::copySourceRegisterToDestinationRegister(sop::processor::processor *proc, char source_processor_register, char destination_processor_register)
{
  uint16_t* src_reg = ProcessorHandler::charRegisterHandler(proc, source_processor_register);
  uint16_t* dst_reg = ProcessorHandler::charRegisterHandler(proc, destination_processor_register);
  (*dst_reg) = (*src_reg);
}

void sop::processor::ProcessorHandler::addSourceRegisterToDestinationRegister(sop::processor::processor *proc, char source_processor_register, char destination_processor_register)
{
  uint16_t* src_reg = ProcessorHandler::charRegisterHandler(proc, source_processor_register);
  uint16_t* dst_reg = ProcessorHandler::charRegisterHandler(proc, destination_processor_register);
  (*dst_reg) += (*src_reg);
}

void sop::processor::ProcessorHandler::subtractDestinationRegisterBySourceRegister(sop::processor::processor *proc, char source_processor_register, char destination_processor_register)
{
  uint16_t* src_reg = ProcessorHandler::charRegisterHandler(proc, source_processor_register);
  uint16_t* dst_reg = ProcessorHandler::charRegisterHandler(proc, destination_processor_register);
  (*dst_reg) -= (*src_reg);
}

void sop::processor::ProcessorHandler::incrementInstructionPointer(sop::processor::processor *proc)
{
  proc->ip++;
}

void sop::processor::ProcessorHandler::setInstructionPointer(sop::processor::processor *proc, short value)
{
  proc->ip = value;
}

void sop::processor::ProcessorHandler::increasInstructionPointerBy(sop::processor::processor *proc, short value)
{
  proc->ip += value;
}

void sop::processor::ProcessorHandler::loadWhereCodeSegmentStarts(/* process *p */ sop::processor::processor *proc)
{
  // to be done
}

void sop::processor::ProcessorHandler::setSignFlag(sop::processor::processor *proc)
{
  proc->sign_flag = true;
}

void sop::processor::ProcessorHandler::setZeroFlag(sop::processor::processor *proc)
{
  proc->zero_flag = true;
}

void sop::processor::ProcessorHandler::unsetSignFlag(sop::processor::processor *proc)
{
  proc->sign_flag = false;
}

void sop::processor::ProcessorHandler::unsetZeroFlag(sop::processor::processor *proc)
{
  proc->zero_flag = false;
}

// to be done
//void sop::processor::ProcessorHandler::addToStack(); 
//void sop::processor::ProcessorHandler::popFromStack 
void sop::processor::ProcessorHandler::clearStack(sop::processor::processor *proc)
{
  while (!proc->processor_stack.size()==0)
  {
    proc->processor_stack.pop();
  }
}