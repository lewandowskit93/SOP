#include "./sop/processor/processor.h"
sop::processor::processor::processor():
        a(0x0000), 
        b(0x0000), 
        c(0x0000), 
        d(0x0000), 
        ip(0x0000),
        sign_flag(0x0000),
        zero_flag(0x0000)
{
}
        

sop::processor::processor::~processor()
{
}

void sop::processor::ProcessorHandler::multipliesAandB(sop::processor::processor *proc)
{
  proc->c = proc->a * proc->b;
}

void sop::processor::ProcessorHandler::dividesAandB(sop::processor::processor *proc)
{
  proc->c = proc->a / proc->b;
  proc->d = proc->a % proc->b;
}

void sop::processor::ProcessorHandler::printOutProcessorState(sop::processor::processor *proc)
{
  std::cout<<"REG A: "<< std::hex <<proc->a<<std::endl;
  std::cout<<"REG B: "<< std::hex <<proc->b<<std::endl;
  std::cout<<"REG C: "<< std::hex <<proc->c<<std::endl;
  std::cout<<"REG D: "<< std::hex <<proc->d<<std::endl;
  std::cout<<"IP: "<< std::hex <<proc->ip<<std::endl;
  std::cout<<"ZERO FLAG: ";
  
  if (proc->zero_flag) std::cout<<"TRUE"<<std::endl;
  else std::cout<<"FALSE"<<std::endl;
  std::cout<<"SIGN FLAG: ";
  if (proc->sign_flag) std::cout<<"TRUE"<<std::endl;
  else std::cout<<"FALSE"<<std::endl;
  std::cout<<"STACK SIZE: "<< std::hex <<proc->processor_stack.size()<<std::endl;
  if (proc->processor_stack.size()>0)
    std::cout<<"STACK TOP: "<< std::hex <<proc->processor_stack.top()<<std::endl;


}

void sop::processor::ProcessorHandler::clearProcessor(sop::processor::processor *proc)
{
  proc->a = 0x0000;
  proc->b = 0x0000;
  proc->c = 0x0000;
  proc->d = 0x0000;
  proc->cs = 0x0000;
  proc->ip = 0x0000;
  ProcessorHandler::clearStack(proc);
  //proc->quant_time = 3;//Moved to executive_unit
  proc->sign_flag = 0x0000;
  proc->zero_flag = 0x0000;
}
uint16_t sop::processor::ProcessorHandler::softCharRegisterHandler(sop::processor::processor *proc, char processor_register)
{
  if (processor_register == 'a' || processor_register == 'A')
  {
    return proc->a;
  }
  else if (processor_register == 'b' || processor_register == 'B')
  {
    return proc->b;
  }
  else if (processor_register == 'c' || processor_register == 'C')
  {
    return proc->c;
  }
  else if (processor_register == 'd' || processor_register == 'D')
  {
    return proc->d;
  }
  else throw "Bad register";
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
  if (*reg == 0x0000) ProcessorHandler::setZeroFlag(proc);
}

void sop::processor::ProcessorHandler::registerDecrement(sop::processor::processor *proc, char processor_register)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  --(*reg);
  if (*reg == 0x0000) ProcessorHandler::setZeroFlag(proc);
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
  if (*reg == 0x0000)
    sop::processor::ProcessorHandler::setZeroFlag(proc);
}

void sop::processor::ProcessorHandler::setRegisterField(sop::processor::processor *proc, char processor_register, uint16_t value)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  (*reg) = value;
}
void sop::processor::ProcessorHandler::compareRegisters(sop::processor::processor *proc, char one, char two)
{
  if (ProcessorHandler::charRegisterHandler(proc,one) == ProcessorHandler::charRegisterHandler(proc,two)) ProcessorHandler::setZeroFlag(proc);
  else if (one < two) ProcessorHandler::setSignFlag(proc);
  else {
    ProcessorHandler::unsetSignFlag(proc);
    ProcessorHandler::unsetZeroFlag(proc);
  }
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
  // to be done or not.. dunno yet
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

void sop::processor::ProcessorHandler::clearStack(sop::processor::processor *proc)
{
  while (!proc->processor_stack.size()==0)
  {
    proc->processor_stack.pop();
  }
}

void sop::processor::ProcessorHandler::addValueToStack(sop::processor::processor *proc, uint16_t value)
{
  proc->processor_stack.push(value);
}

void sop::processor::ProcessorHandler::addRegisterToStack(sop::processor::processor *proc, char reg)
{
  uint16_t r = softCharRegisterHandler(proc,reg);
  proc->processor_stack.push(r);
}


void sop::processor::ProcessorHandler::popFromStack(sop::processor::processor *proc, char processor_register)
{
  uint16_t *reg = charRegisterHandler(proc,processor_register);
  *reg = proc->processor_stack.top();
  proc->processor_stack.pop();
}

void sop::processor::ProcessorHandler::bitwiseAND(sop::processor::processor *proc, char reg_one, char reg_two)
{
  uint16_t *reg_st = charRegisterHandler(proc,reg_one);
  uint16_t *reg_nd = charRegisterHandler(proc,reg_two);
  *reg_st = *reg_st & *reg_nd; 
  if (*reg_st == 0) setZeroFlag(proc);
}

void sop::processor::ProcessorHandler::bitwiseOR(sop::processor::processor *proc, char reg_one, char reg_two)
{
  uint16_t *reg_st = charRegisterHandler(proc,reg_one);
  uint16_t *reg_nd = charRegisterHandler(proc,reg_two);
  *reg_st = *reg_st | *reg_nd; 
  if (*reg_st == 0) setZeroFlag(proc);
}

void sop::processor::ProcessorHandler::bitNEG(sop::processor::processor *proc, char reg)
{
  uint16_t *reg_t = charRegisterHandler(proc,reg);
  *reg_t = ~*reg_t;
  if (*reg_t == 0) setZeroFlag(proc);
}

void sop::processor::ProcessorHandler::doJMP(processor *proc, uint16_t offset)
{
  proc->ip = offset;
}
void sop::processor::ProcessorHandler::doJIZ(processor *proc, uint16_t offset)
{
  if (proc->zero_flag)
    proc->ip = offset;
}
void sop::processor::ProcessorHandler::doJNZ(processor *proc, uint16_t offset)
{
  if (!proc->zero_flag)
    proc->ip = offset;
}
void sop::processor::ProcessorHandler::doJIA(processor *proc, uint16_t offset)
{
 if (proc->a > proc->b)
    proc->ip = offset; 
}
void sop::processor::ProcessorHandler::doJAE(processor *proc, uint16_t offset)
{
  if (proc->a >= proc->b)
    proc->ip = offset;
}
void sop::processor::ProcessorHandler::doJIB(processor *proc, uint16_t offset)
{
  if (proc->a < proc->b)
    proc->ip = offset; 
}
void sop::processor::ProcessorHandler::doJBE(processor *proc, uint16_t offset)
{
  if (proc->a <= proc->b)
    proc->ip = offset; 
}
void sop::processor::ProcessorHandler::saveOnYoungestByte(processor *proc, uint16_t byte)
{
  //uint8_t value_to_be_set = sop::StringConverter::convertStringToHex<uint8_t>(byte);
  uint16_t *ptr = &proc->a;
  uint8_t *wskByte = (uint8_t*) ptr;
  wskByte[0] = (uint8_t) byte;
}
void sop::processor::ProcessorHandler::readOneByteFromInputAndSavesItOnYoungestByte(processor *proc)
{
  std::cout<<"____INPUT____\n"; 
  uint16_t input = 513;
  while (input>255)
  {
    std::cout << "Program is reading input data the value must be lower than 255: ";
    std::cin>>input;
  }
  saveOnYoungestByte(proc,input);
}
void sop::processor::ProcessorHandler::printsOutYoungestByte(processor *proc)
{
  uint16_t *ptr = &proc->a;
  uint8_t *wskByte = (uint8_t*) ptr;
  std::cout << (uint16_t) wskByte[0] << std::endl;
}
void sop::processor::ProcessorHandler::printsOutYoungestByteAsASCII(processor *proc)
{
  uint16_t *ptr = &proc->a;
  uint8_t *wskByte = (uint8_t*) ptr;
  std::cout << wskByte[0] << std::endl;
}
void sop::processor::ProcessorHandler::printsOutRegisterWithoutSign(processor *proc)
{
  std::cout << proc->a << std::endl;
}
void sop::processor::ProcessorHandler::printsOutRegisterWithSign(processor *proc)
{
  std :: cout << (int16_t) proc->a << std::endl;
}