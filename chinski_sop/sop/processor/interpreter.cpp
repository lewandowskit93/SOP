#include "./sop/processor/interpreter.h"
char sop::interpreter::InterpreterHandler::getByteFromMemory(sop::processor::processor *proc)
{
  //char x = sop::memory::.. pobranie byte'u danych od Fiszera  
  //return x;
  return '0';
}

void sop::interpreter::InterpreterHandler::interpretLine()
{
  /*
  switch (program_line.at(0))
  {
  case 'A':
  sop::processor::wykonaj_na_procesorze (*processor);
  case 'C':
  case 'D':
  case 'E':
  case 'I':
  case 'M':
  case 'N':

  }
  etc..
  */ 

  _program_line = "";
}

void sop::interpreter::InterpreterHandler::buildProgramLine()
{

  char getChar = 's'; 
  while (getChar!='\n')
  {
    //getChar = getByteFromMemory(..)
    //program_line+ = getChar;
  }

  interpretLine();
}
  