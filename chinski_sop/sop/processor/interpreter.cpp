#include "./sop/processor/interpreter.h"

char sop::interpreter::InterpreterHandler::getByteFromMemory(boost::shared_ptr<sop::process::Process> p)
{
  //char x = sop::memory::.. pobranie byte'u danych od Fiszera  
  //char x = sop::memory::LogicalMemory::read(
  //return x;
  return '0';
}


std::string sop::interpreter::InterpreterHandler::interpretLine(boost::shared_ptr<sop::process::Process> p)
{
  
  pickCommandPart(_program_line);
  if (_program_line.size() > 3)
    pickDataPart(_program_line);
  p->procek.ip += _program_line.size()+1;
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
    uint16_t value = sop::StringConverter::convertStringToHex<uint16_t>(s_value);
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
    uint16_t value = sop::StringConverter::convertStringToHex<uint16_t>(s_value);
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
    char dst = _data_part[2]; //todo
    sop::processor::ProcessorHandler::compareRegisters(&p->procek,src,dst);    
  }
  else if (_command_part == "OR")
  {
    char first = _data_part[0];
    char second = _data_part[2];
    sop::processor::ProcessorHandler::bitwiseOR(&p->procek,first,second);  
  }
  else if (_command_part == "AND")
  {
    char first = _data_part[0];
    char second = _data_part[2];
    sop::processor::ProcessorHandler::bitwiseAND(&p->procek,first,second); 
  }
  else if (_command_part == "XOR")
  {
    /////////// narazie nie
  }
  else if (_command_part == "NEG")
  {
    char first = _data_part[0];
    sop::processor::ProcessorHandler::bitNEG(&p->procek,first); 
  }
  else if (_command_part == "SHR")
  {
    /////////// narazie nie
  }
  else if (_command_part == "SHL")
  {
    /////////// narazie nie
  }
  else if (_command_part == "MOV")
  {
    char reg = _data_part[0];
    std::string s_value = _data_part.substr(2);
    uint16_t value = sop::StringConverter::convertStringToHex<uint16_t>(s_value);
    sop::processor::ProcessorHandler::setRegisterField(&p->procek,reg,value);
  }
  else if (_command_part == "MOR")
  {
    char src = _data_part[0];
    char dst = _data_part[2];
    sop::processor::ProcessorHandler::copySourceRegisterToDestinationRegister(&p->procek,dst,src);
  }
  else if (_command_part == "PUS")
  {
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::addRegisterToStack(&p->procek,reg);
  }
  else if (_command_part == "PUSF")
  {
    //////////narazie nie
  }
  else if (_command_part == "POP")
  {
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::popFromStack(&p->procek,reg);
  }
  else if (_command_part == "POF")
  {
    ////narazie nie
  }
  else if (_command_part == "MOM")
  {
    //MOM - wczytuje zaw. z pamieci pod rej. D do rej. C
    //funkcja fiszerowa
  }
  else if (_command_part == "CPY")
  {
    //CPY  - kopiuje zaw. rej. C pod adres wskazany pod rej. D
    //funkcja fiszerowa
  }
  else if (_command_part == "EXT")
  {
    return "EXT";//+value // dodaæ wartoœæ z jak¹ siê program zakonczyl 
    //pobraæ wartosc zakocnzenie programu od Michala
  }
  else if (_command_part == "CAL")
  {
    //////narazie nie
  }
  else if (_command_part == "RET")
  {
    //////narazie nie
  }
  else if (_command_part == "JMP")
  {
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJMP(&p->procek);
  }
  else if (_command_part == "JIZ")
  {
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJIZ(&p->procek);
  }
  else if (_command_part == "JNZ")
  {
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJNZ(&p->procek);
  }
  else if (_command_part == "JIA")
  {
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJIA(&p->procek);
  }
  else if (_command_part == "JAE")
  {
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJAE(&p->procek);
  }
  else if (_command_part == "JIB")
  {
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJIB(&p->procek);
  }
  
  else if (_command_part == "JBE")
  {
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJNZ(&p->procek);
  }

  else if (_command_part == "REB")
  {
    sop::processor::ProcessorHandler::readOneByteFromInputAndSavesItOnYoungestByte(&p->procek);
  }
  else if (_command_part == "WRB")
  {
    sop::processor::ProcessorHandler::printsOutYoungestByte(&p->procek);
  }
  else if (_command_part == "WRC")
  {
    sop::processor::ProcessorHandler::printsOutYoungestByteAsASCII(&p->procek);
  }
  else if (_command_part == "WRI")
  {
    sop::processor::ProcessorHandler::printsOutRegisterWithSign(&p->procek);
  }
  else if (_command_part == "WRU")
  {
    sop::processor::ProcessorHandler::printsOutRegisterWithoutSign(&p->procek);
  }
  else if (_command_part == "FRK")
  {

  }
  else if (_command_part == "EXC")
  {

  }
  else if (_command_part == "WAT")
  {

  }
  //FRK wywolywane od M. Pietrzaka, 0 gdy potomek, PID potomka gdy dziecko
  //PID do rej A

  //EXC otwiera plik o nazwie zapisanej w pamieci pod adresem pam w rej B
  //w rejestr C 0- nie udalo sie albo 1  - udalo


  //WAT czeka na zakonczenie procesu potomka o PID zap w rej A.
  //Pietrzak w tym momencie przeszuka ziomka o podanym PID, jezeli go nei znajdzie -> kill him, albo nie bedzie potomkiem -> kill him
  //w przeciwnim razie czekam az Michal mi go wrzuci spowrotem
  //na WAT wyrzucam z schedulera tak jak EXT
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
void sop::interpreter::InterpreterHandler::buildProgramLine(boost::shared_ptr<sop::process::Process> p)
{

  char getChar = '\n'; 
  while (getChar!='\n')
  {
    //p->procek.ip
    //getChar = getByteFromMemory(..)
    //program_line+ = getChar;
  }
  _program_line = "REB";
}
  
void sop::interpreter::InterpreterHandler::interpreterReset()
{
  _command_part = "";
  _data_part = "";
  _program_line = "";
}