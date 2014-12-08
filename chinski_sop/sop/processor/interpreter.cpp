#include "./sop/processor/interpreter.h"

char sop::interpreter::InterpreterHandler::getByteFromMemory(sop::process::Process *p)
{
  //char x = sop::memory::.. pobranie byte'u danych od Fiszera  
  //return x;
  return '0';
}


std::string sop::interpreter::InterpreterHandler::interpretLine(sop::process::Process *p)
{
  
  pickCommandPart(_program_line);
  if (_program_line.size() > 3)
    pickDataPart(_program_line);
  p->procek.ip += _program_line.size();
  if (_command_part == "ADD")
  {
    char src = _data_part[0];
    char dst = _data_part[2];
    sop::processor::ProcessorHandler::addSourceRegisterToDestinationRegister(&p->procek,dst,src);    
  }
  else if (_command_part == "ADV")
  {
    char reg = _data_part[0];
    std::string s_value = _data_part.substr(2);
    uint16_t value = sop::StringConverter::convertStringTo<uint16_t>(s_value);
    sop::processor::ProcessorHandler::registerIncrementByValue(&p->procek,reg,value);
  }
  else if (_command_part == "SUB")
  {
    char src = _data_part[0];
    char dst = _data_part[2];
    sop::processor::ProcessorHandler::subtractDestinationRegisterBySourceRegister(&p->procek,dst,src);   
  }
  else if (_command_part == "SUV")
  {
    char reg = _data_part[0];
    std::string s_value = _data_part.substr(2);
    uint16_t value = sop::StringConverter::convertStringTo<uint16_t>(s_value);
    sop::processor::ProcessorHandler::registerDecrementByValue(&p->procek,reg,value);   
    
  }
  else if (_command_part == "MUL")
  {
    sop::processor::ProcessorHandler::multipliesAandB(&p->procek);
  }
  else if (_command_part == "DIV")
  {
    sop::processor::ProcessorHandler::dividesAandB(&p->procek);
  }
  else if (_command_part == "INC")
  {
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::registerIncrement(&p->procek,reg);
  }
  else if (_command_part == "DEC")
  {
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::registerDecrement(&p->procek,reg);
  }
  else if (_command_part == "CMP")
  {
    char src = _data_part[0];
    char dst = _data_part[2];
    sop::processor::ProcessorHandler::compareRegisters(&p->procek,src,dst);    
  }
  else if (_command_part == "OR")
  {

  }
  else if (_command_part == "AND")
  {

  }
  else if (_command_part == "XOR")
  {

  }
  else if (_command_part == "NEG")
  {

  }
  else if (_command_part == "SHR")
  {

  }
  else if (_command_part == "SHL")
  {

  }
  else if (_command_part == "MOV")
  {
    char reg = _data_part[0];
    std::string s_value = _data_part.substr(2);
    uint16_t value = sop::StringConverter::convertStringTo<uint16_t>(s_value);
    sop::processor::ProcessorHandler::setRegisterField(&p->procek,reg,value);
  }
  else if (_command_part == "MOR")
  {
    char src = _data_part[0];
    char dst = _data_part[2];
    sop::processor::ProcessorHandler::copySourceRegisterToDestinationRegister(&p->procek,dst,src);
  }
  else if (_command_part == "PUSH")
  {
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::addRegisterToStack(&p->procek,reg);
  }
  else if (_command_part == "PUSF")
  {

  }
  else if (_command_part == "POP")
  {
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::popFromStack(&p->procek,reg);
  }
  else if (_command_part == "POF")
  {

  }
  else if (_command_part == "EXT")
  {
    return "EXT";
  }
  else if (_command_part == "CAL")
  {

  }
  else if (_command_part == "RET")
  {

  }
  else if (_command_part == "JMP")
  {

  }
  else if (_command_part == "JIZ")
  {

  }
  else if (_command_part == "JNZ")
  {

  }
  else if (_command_part == "REB")
  {

  }
  else if (_command_part == "WRB")
  {

  }
  else if (_command_part == "WRC")
  {

  }
  else if (_command_part == "WRI")
  {

  }
  else if (_command_part == "WRU")
  {

  }
  //etc..
  _program_line = "";
  return "";
}

void sop::interpreter::InterpreterHandler::pickDataPart(std::string s)
{
  if (s.size() > 3)
  _data_part = s.substr(4);
}

void sop::interpreter::InterpreterHandler::pickCommandPart(std::string s)
{
  _command_part = s.substr(0,3);
}

void sop::interpreter::InterpreterHandler::printCommandPart()
{
  std::cout<<_command_part<<std::endl;
}

void sop::interpreter::InterpreterHandler::printDataPart()
{
  std::cout<<_command_part<<std::endl;
}

std::string sop::interpreter::InterpreterHandler::getDataPart()
{
  return _data_part;
}

std::string sop::interpreter::InterpreterHandler::getCommandPart()
{
  return _command_part;
}
void sop::interpreter::InterpreterHandler::buildProgramLine(sop::process::Process *p)
{

  char getChar = '\n'; 
  while (getChar!='\n')
  {
    //p->procek.ip
    //getChar = getByteFromMemory(..)
    //program_line+ = getChar;
  }
  _program_line = "ADD A,B";
}
  
void sop::interpreter::InterpreterHandler::interpreterReset()
{
  _command_part = "";
  _data_part = "";
  _program_line = "";
}