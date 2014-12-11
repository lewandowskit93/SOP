#include "./sop/processor/processor.h"
sop::processor::processor::processor():
        a(0), 
        b(0), 
        c(0), 
        d(0), 
        ip(0),
        sign_flag(0),
        zero_flag(0)
{
}
        
sop::processor::processor::~processor()
{
}

//ADD
void sop::processor::ProcessorHandler::addSourceRegisterToDestinationRegister(sop::processor::processor *proc, char source_processor_register, char destination_processor_register)
{
  uint16_t* src_reg = ProcessorHandler::charRegisterHandler(proc, source_processor_register);
  uint16_t* dst_reg = ProcessorHandler::charRegisterHandler(proc, destination_processor_register);
  (*dst_reg) += (*src_reg);
  if (*dst_reg == 0) setZeroFlag(proc);
  else unsetZeroFlag(proc);
}

//ADV
void sop::processor::ProcessorHandler::registerIncrementByValue(sop::processor::processor *proc, char processor_register, uint16_t value)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  (*reg) += value;
  if (*reg == 0) ProcessorHandler::setZeroFlag(proc);
  else ProcessorHandler::unsetZeroFlag(proc);
}

//SUB
void sop::processor::ProcessorHandler::subtractDestinationRegisterBySourceRegister(sop::processor::processor *proc, char source_processor_register, char destination_processor_register)
{
  uint16_t* src_reg = ProcessorHandler::charRegisterHandler(proc, source_processor_register);
  uint16_t* dst_reg = ProcessorHandler::charRegisterHandler(proc, destination_processor_register);
  if (*src_reg == *dst_reg)
  {
    ProcessorHandler::setZeroFlag(proc);
    ProcessorHandler::unsetSignFlag(proc);
  }
  else if (*src_reg < *dst_reg)
  {
    ProcessorHandler::setSignFlag(proc);
    ProcessorHandler::unsetZeroFlag(proc);
  }
  else 
  {
    ProcessorHandler::unsetSignFlag(proc);
    ProcessorHandler::unsetZeroFlag(proc);
  }
  (*dst_reg) -= (*src_reg);
}

//SUV
void sop::processor::ProcessorHandler::registerDecrementByValue(sop::processor::processor *proc, char processor_register, uint16_t value)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  if (*reg == value)
  {
    ProcessorHandler::setZeroFlag(proc);
    ProcessorHandler::unsetSignFlag(proc);
  }
  else if (*reg < value)
  {
    ProcessorHandler::setSignFlag(proc);
    ProcessorHandler::unsetZeroFlag(proc);
  }
  else 
  {
    ProcessorHandler::unsetSignFlag(proc);
    ProcessorHandler::unsetZeroFlag(proc);
  }
  (*reg) -= value;
}

//MUL
void sop::processor::ProcessorHandler::multipliesAandB(sop::processor::processor *proc)
{
  proc->c = proc->a * proc->b;
  if (proc->c == 0)
  {
    ProcessorHandler::setZeroFlag(proc);
  }
  else
    ProcessorHandler::unsetZeroFlag(proc);
}

//DIV
void sop::processor::ProcessorHandler::dividesAandB(sop::processor::processor *proc)
{
  proc->c = proc->a / proc->b;
  proc->d = proc->a % proc->b;
  if (proc->c == 0) ProcessorHandler::setZeroFlag(proc);
  else ProcessorHandler::unsetZeroFlag(proc);
}

//INC
void sop::processor::ProcessorHandler::registerIncrement(sop::processor::processor *proc, char processor_register)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  ++(*reg);
  if (*reg == 0) ProcessorHandler::setZeroFlag(proc);
  else ProcessorHandler::unsetZeroFlag(proc);
}

//DEC
void sop::processor::ProcessorHandler::registerDecrement(sop::processor::processor *proc, char processor_register)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  --(*reg);
  if (*reg == 0) ProcessorHandler::setZeroFlag(proc);
  else ProcessorHandler::unsetZeroFlag(proc);
}

//CMP
void sop::processor::ProcessorHandler::compareRegisters(sop::processor::processor *proc, char one, char two)
{
  uint16_t *reg_one = ProcessorHandler::charRegisterHandler(proc,one);
  uint16_t *reg_two = ProcessorHandler::charRegisterHandler(proc,two);
  
  if (one == two)
  {
    ProcessorHandler::setZeroFlag(proc);
    ProcessorHandler::unsetSignFlag(proc);
  }
  else if (one < two)
  {
    ProcessorHandler::setSignFlag(proc);
    ProcessorHandler::unsetZeroFlag(proc);
  }
  else 
  {
    ProcessorHandler::unsetSignFlag(proc);
    ProcessorHandler::unsetZeroFlag(proc);
  }
}

//OR
void sop::processor::ProcessorHandler::bitwiseOR(sop::processor::processor *proc, char reg_one, char reg_two)
{
  uint16_t *reg_st = charRegisterHandler(proc,reg_one);
  uint16_t *reg_nd = charRegisterHandler(proc,reg_two);
  *reg_st = *reg_st | *reg_nd; 
  if (*reg_st == 0) setZeroFlag(proc);
  else unsetZeroFlag(proc);
}

//AND
void sop::processor::ProcessorHandler::bitwiseAND(sop::processor::processor *proc, char reg_one, char reg_two)
{
  uint16_t *reg_st = charRegisterHandler(proc,reg_one);
  uint16_t *reg_nd = charRegisterHandler(proc,reg_two);
  *reg_st = *reg_st & *reg_nd; 
  if (*reg_st == 0) setZeroFlag(proc);
  else unsetZeroFlag(proc);
}

//NEG
void sop::processor::ProcessorHandler::bitNEG(sop::processor::processor *proc, char reg)
{
  uint16_t *reg_t = charRegisterHandler(proc,reg);
  *reg_t = ~*reg_t;
  if (*reg_t == 0) setZeroFlag(proc);
  else unsetZeroFlag(proc);
}

//MOV
void sop::processor::ProcessorHandler::setRegisterField(sop::processor::processor *proc, char processor_register, uint16_t value)
{
  uint16_t* reg = ProcessorHandler::charRegisterHandler(proc, processor_register);
  (*reg) = value;
}

//MOR
void sop::processor::ProcessorHandler::copySourceRegisterToDestinationRegister(sop::processor::processor *proc, char source_processor_register, char destination_processor_register)
{
  uint16_t* src_reg = ProcessorHandler::charRegisterHandler(proc, source_processor_register);
  uint16_t* dst_reg = ProcessorHandler::charRegisterHandler(proc, destination_processor_register);
  (*dst_reg) = (*src_reg);
}

//PUS
void sop::processor::ProcessorHandler::addRegisterToStack(sop::processor::processor *proc, char reg)
{
  uint16_t r = softCharRegisterHandler(proc,reg);
  proc->processor_stack.push(r);
}

//POP
void sop::processor::ProcessorHandler::popFromStack(sop::processor::processor *proc, char processor_register)
{
  uint16_t *reg = charRegisterHandler(proc,processor_register);
  *reg = proc->processor_stack.top();
  proc->processor_stack.pop();
}

//EXT...

//CAL
void sop::processor::ProcessorHandler::savesReturnAdresOnStack(sop::processor::processor *proc)
{
  proc->processor_stack.push(proc->ip);
  proc->ip = proc->d;
}

//RET
void sop::processor::ProcessorHandler::loadsValueFromStackAndJumpOnIt(sop::processor::processor *proc)
{
  proc->ip = proc->processor_stack.top();
  proc->processor_stack.pop();
}

//JMP
void sop::processor::ProcessorHandler::doJMP(processor *proc)
{
  uint16_t offset = proc->d;
  proc->ip = offset;
}

//JIZ
void sop::processor::ProcessorHandler::doJIZ(processor *proc)
{
  uint16_t offset = proc->d;
  if (proc->zero_flag)
    proc->ip = offset;
}

//JNZ
void sop::processor::ProcessorHandler::doJNZ(processor *proc)
{
  uint16_t offset = proc->d;
  if (!proc->zero_flag)
    proc->ip = offset;
}

//JIA
void sop::processor::ProcessorHandler::doJIA(processor *proc)
{
  uint16_t offset = proc->d;
  if (!proc->sign_flag)
    proc->ip = offset; 
}

//JAE
void sop::processor::ProcessorHandler::doJAE(processor *proc)
{
  uint16_t offset = proc->d;
  if (!proc->sign_flag || proc->zero_flag)
    proc->ip = offset;
}

//JIB
void sop::processor::ProcessorHandler::doJIB(processor *proc)
{
  uint16_t offset = proc->d;
  if (proc->sign_flag)
    proc->ip = offset; 
}

//JBE
void sop::processor::ProcessorHandler::doJBE(processor *proc)
{
  uint16_t offset = proc->d;
  if (proc->sign_flag || proc->zero_flag)
    proc->ip = offset; 
}

//REB FUNC
void sop::processor::ProcessorHandler::saveOnYoungestByte(processor *proc, uint16_t byte)
{
  //uint8_t value_to_be_set = sop::StringConverter::convertStringToHex<uint8_t>(byte);
  uint16_t *ptr = &proc->b;
  uint8_t *wskByte = (uint8_t*) ptr;
  wskByte[0] = (uint8_t) byte;
}

//REB
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

//WRB
void sop::processor::ProcessorHandler::printsOutYoungestByte(processor *proc)
{
  uint16_t *ptr = &proc->b;
  uint8_t *wskByte = (uint8_t*) ptr;
  std::cout << (uint16_t) wskByte[0] << std::endl;
}

//WRC
void sop::processor::ProcessorHandler::printsOutYoungestByteAsASCII(processor *proc)
{
  uint16_t *ptr = &proc->b;
  uint8_t *wskByte = (uint8_t*) ptr;
  std::cout << wskByte[0] << std::endl;
}

//WRI
void sop::processor::ProcessorHandler::printsOutRegisterWithoutSign(processor *proc)
{
  std::cout << proc->b << std::endl;
}

//WRU
void sop::processor::ProcessorHandler::printsOutRegisterWithSign(processor *proc)
{
  std :: cout << (int16_t) proc->b << std::endl;
}

//SWB
void sop::processor::ProcessorHandler::swapBytes(processor *proc, char reg)
{
  uint16_t *ptr_reg = ProcessorHandler::charRegisterHandler(proc,reg);
  uint8_t *bytes = (uint8_t *) ptr_reg;
  uint8_t buf = bytes[0]; //younger byte
  bytes[0] = bytes[1];
  bytes[1] = buf;
}


//print shell cmd
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


//mby will clear processor
void sop::processor::ProcessorHandler::clearProcessor(sop::processor::processor *proc)
{
  proc->a = 0;
  proc->b = 0;
  proc->c = 0;
  proc->d = 0;
  proc->ip = 0;
  ProcessorHandler::clearStack(proc);
  //proc->quant_time = 3;//Moved to executive_unit
  proc->sign_flag = 0;
  proc->zero_flag = 0;
}

//handler
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

//handler
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












