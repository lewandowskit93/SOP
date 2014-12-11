#include <iostream>
#include <cctype>
#include <sstream>
#include ".\sop\system\shell.h"
#include ".\sop\system\kernel.h"

sop::system::Shell::ParsingException::ParsingException():
  sop::Exception()
{
}

sop::system::Shell::ParsingException::ParsingException(const std::string & value):
  sop::Exception(value)
{
}

sop::system::Shell::ParsingException::~ParsingException()
{
}

std::string sop::system::Shell::ParsingException::getClassName() const
{
  return "sop::system::Shell::ParsingException";
}

sop::system::Shell::Shell(sop::system::Kernel * kernel):
  sop::Object(),
  _kernel(kernel)
{
}

sop::system::Shell::~Shell()
{
}

std::string sop::system::Shell::getClassName() const
{
  return "sop::system::Shell";
}

sop::system::Kernel * sop::system::Shell::getKernel()
{
  return _kernel;
}

const sop::system::Kernel * sop::system::Shell::getKernel() const
{
  return _kernel;
}

void sop::system::Shell::step()
{
  _kernel->getLogger()->logShell(sop::logger::Logger::Level::FINE, "Executing next shell step.");
  if(!std::cin.good())
  {
    _kernel->getLogger()->logShell(sop::logger::Logger::Level::INFO, "End of standard intput. Shutting down the kernel.");
    _kernel->shutdown();
    return;
  }
  std::cout<<"$ ";
  std::getline(std::cin,_last_input_line);
  _kernel->getLogger()->logShell(sop::logger::Logger::Level::FINER, "Shell command readed.");
  _kernel->getLogger()->logShell(sop::logger::Logger::Level::FINEST,"Readed line: "+_last_input_line);
  try
  {
    std::vector<std::string> parameters = parse(_last_input_line);
    if(parameters.size()==0)
    {
        _kernel->getLogger()->logShell(sop::logger::Logger::Level::FINEST,"Empty shell command.");
        return;
    }
    std::string command=parameters[0];
    if(_commands.count(command)==1)
    {
      _kernel->getLogger()->logShell(sop::logger::Logger::Level::INFO, "Executing shell command: "+command);
      std::string result;
      (*_commands[command])(toVectorOfConst(parameters));
      _kernel->getLogger()->logShell(sop::logger::Logger::Level::INFO, "Shell command execution finished.");
    }
    else
    {
      _kernel->getLogger()->logShell(sop::logger::Logger::Level::FINE,"Unknown shell command: "+command);
    }
  }
  catch(const ParsingException & exception)
  {
    _kernel->getLogger()->logShell(sop::logger::Logger::Level::FINE,"Command parsing exception: "+exception.getValue());
  }
}

bool sop::system::Shell::registerCommand(const std::string & command, CommandHandler * function)
{
  return doRegisterCommand(command,function);
}


std::vector<std::string> sop::system::Shell::parse(const std::string & line)
{
  bool is_quote_opened=false;
  bool escape_sign=false;
  std::string current_parameter;
  std::vector<std::string> arguments;

  for(std::string::size_type i=0; i<line.length();++i)
  {
    if(escape_sign)
    {
      if(line[i]=='"' || line[i]=='\\')
      {
        current_parameter+=line[i];
      }
      else
      {
        std::stringstream exception_message;
        exception_message<<"Unknown special sign \\"<<line[i]<<" at: "<<i<<". Only \\ and \" allowed after escape sign.";
        throw ParsingException(exception_message.str());
      }
      escape_sign=false;
    }
    else if(isspace(line[i]))
    {
      if(is_quote_opened)
      {
        current_parameter+=line[i];
      }
      else
      {
        if(current_parameter.length()>0)
        {
          arguments.push_back(current_parameter);
        }
        current_parameter.clear();
      }
    }
    else if(line[i]=='"')
    {
      if(is_quote_opened)
      {
        is_quote_opened=false;
      }
      else
      {
        is_quote_opened=true;
        if(current_parameter.length()>0)
        {
          std::stringstream exception_message;
          exception_message<<"\" is not first sign of the parameter "<<" at: "<<(i+1)<<".";
          throw ParsingException(exception_message.str());
        }
      }
    }
    else if(line[i]=='\\')
    {
      if(is_quote_opened)
      {
        escape_sign=true;
      }
      else
      {
        current_parameter+=line[i];
      }
    }
    else {
      current_parameter+=line[i];
    }
  }
  
  if(current_parameter.length()>0)
  {
    arguments.push_back(current_parameter);
  }

  return arguments;
}

std::vector<const std::string> sop::system::Shell::toVectorOfConst(const std::vector<std::string> & vect)
{
  std::vector<const std::string> const_vect;
  const_vect.reserve(vect.size());
  const_vect.insert(const_vect.begin(),vect.begin(),vect.end());
  return const_vect;
}

std::vector<std::string> sop::system::Shell::getRegisteredCommands()
{
  std::vector<std::string> commands;
  commands.reserve(_commands.size());
  std::map<std::string, boost::shared_ptr<CommandHandlerSignal>>::iterator it;
  for(it = _commands.begin(); it!=_commands.end();++it)
  {
    commands.push_back(it->first);
  }
  return commands;
}

bool sop::system::Shell::hasParam(const std::vector<const std::string> & param, const std::string & param_name)
{
  for(std::vector<std::string>::size_type i = 0; i<param.size();++i)
  {
    if(param[i]==param_name) return true;
  }
  return false;
}

std::string sop::system::Shell::getParamValue(const std::vector<const std::string> & param, const std::string & param_name)
{
  for(std::vector<std::string>::size_type i = 0; i<(param.size()-1);++i)
  {
    if(param[i]==param_name)
    {
      return param[i+1];
    }
  }
  return "";
}