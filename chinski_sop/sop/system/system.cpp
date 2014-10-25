#include ".\sop\system\system.h"
#include ".\sop\logger\console_logger.h"
#include ".\sop\system\exceptions\module_exceptions.h"
#include ".\sop\system\exceptions\system_exceptions.h"

sop::system::System::System():
  sop::Object(),
  _logger(new sop::logger::ConsoleLogger),
  _shell(this),
  _users_module(this),
  _files_module(this),
  _processes_module(this),
  _memory_module(this),
  _processor_module(this),
  _system_state(State::PRE_INIT)
{
  _modules.reserve(5);
  _modules.push_back(&_users_module);
  _modules.push_back(&_files_module);
  _modules.push_back(&_processes_module);
  _modules.push_back(&_memory_module);
  _modules.push_back(&_processor_module);
}

sop::system::System::System(uint16_t logging_level):
  sop::Object(),
  _logger(new sop::logger::ConsoleLogger(logging_level)),
  _shell(this),
  _users_module(this),
  _files_module(this),
  _processes_module(this),
  _memory_module(this),
  _processor_module(this),
  _system_state(State::PRE_INIT)
{
  _modules.reserve(5);
  _modules.push_back(&_users_module);
  _modules.push_back(&_files_module);
  _modules.push_back(&_processes_module);
  _modules.push_back(&_memory_module);
  _modules.push_back(&_processor_module);
}

sop::system::System::~System()
{
}

std::string sop::system::System::getClassName() const
{
  return "sop::system::System";
}

void sop::system::System::initializeSystem()
{
  _logger->logSystem(sop::logger::Logger::Level::INFO, "Initializing system.");
  if(_system_state==State::PRE_INIT)
  {
    _logger->logSystem(sop::logger::Logger::Level::INFO, "Initializing modules.");
    for(std::vector<sop::system::Module>::size_type i=0; i<_modules.size(); ++i)
    {
      _logger->logSystem(sop::logger::Logger::Level::INFO, "Initializing module: "+_modules[i]->getClassName());
      try
      {
        _modules[i]->initializeModule();
      }
      catch(const sop::system::exceptions::ModuleInitializationException & exception)
      {
        _logger->logSystem(sop::logger::Logger::Level::SEVERE, "Module: "+_modules[i]->getClassName()+" initialization has failed.");
        throw sop::system::exceptions::SystemInitializationException("Module: "+_modules[i]->getClassName()+" initialization has failed.");
      }
      _logger->logSystem(sop::logger::Logger::Level::INFO, "Module: "+_modules[i]->getClassName() + " initialized.");
    }
    _system_state=INITIALIZED;
    _logger->logSystem(sop::logger::Logger::Level::INFO, "System initialized.");
  }
  else
  {
    _logger->logSystem(sop::logger::Logger::Level::SEVERE, "Invalid system state.");
    throw sop::system::exceptions::SystemInitializationException("Invalid system state.");
  }
}

void sop::system::System::run()
{
  _logger->logSystem(sop::logger::Logger::Level::INFO, "Starting system.");
  if(_system_state==State::INITIALIZED)
  {
    _system_state=State::RUNNING;
    _logger->logSystem(sop::logger::Logger::Level::INFO, "System is running.");
    while(!isShuttingDown())
    {
      _logger->logSystem(sop::logger::Logger::Level::INFO, "Executing next shell step.");
      _shell.step();
    }
    _logger->logSystem(sop::logger::Logger::Level::INFO, "Shutting down the system.");
  }
  else
  {
    _logger->logSystem(sop::logger::Logger::Level::SEVERE, "Invalid system state.");
    throw sop::system::exceptions::SystemRunException("Invalid system state.");
  }
}

const sop::logger::Logger * sop::system::System::getLogger() const
{
  return _logger.get();
}

sop::logger::Logger * sop::system::System::getLogger()
{
  return _logger.get();
}

const sop::system::Shell * sop::system::System::getShell() const
{
  return &_shell;
}

sop::system::Shell * sop::system::System::getShell()
{
  return &_shell;
}

sop::users::Module * sop::system::System::getUsersModule()
{
   return &_users_module;
}

const sop::users::Module * sop::system::System::getUsersModule() const
{
  return &_users_module;
}

sop::files::Module * sop::system::System::getFilesModule()
{
  return &_files_module;
}

const sop::files::Module * sop::system::System::getFilesModule() const
{
  return &_files_module;
}

sop::processes::Module * sop::system::System::getProcessesModule()
{
  return &_processes_module;
}

const sop::processes::Module * sop::system::System::getProcessesModule() const
{
  return &_processes_module;
}

sop::memory::Module * sop::system::System::getMemoryModule()
{
  return &_memory_module;
}

const sop::memory::Module * sop::system::System::getMemoryModule() const
{
  return &_memory_module;
}

sop::processor::Module * sop::system::System::getProcessorModule()
{
  return &_processor_module;
}

const sop::processor::Module * sop::system::System::getProcessorModule() const
{
  return &_processor_module;
}

std::vector<sop::system::Module*> sop::system::System::getModules()
{
  return _modules;
}

std::vector<const sop::system::Module*> sop::system::System::getModules() const
{
  std::vector<const sop::system::Module*> modules;
  for(std::vector<sop::system::Module>::size_type i=0; i<_modules.size(); ++i)
  {
    modules.push_back(_modules[i]);
  }
  return modules;
}

void sop::system::System::shutdown() const
{
  _logger->logSystem(sop::logger::Logger::Level::INFO, "Initializing shutdown procedure.");
  if(_system_state==State::RUNNING)
  {
    _system_state=State::SHUTTING_DOWN;
    _logger->logSystem(sop::logger::Logger::Level::INFO, "Shutdown procedure initialized.");
  }
  else if(_system_state!=State::SHUTTING_DOWN)
  {
    _logger->logSystem(sop::logger::Logger::Level::SEVERE, "Invalid system state.");
    throw sop::system::exceptions::SystemShutdownException("Invalid system state.");
  }
  else{
    _logger->logSystem(sop::logger::Logger::Level::INFO, "System is already shutting down.");
  }
}

bool sop::system::System::isShuttingDown() const
{
  return _system_state==State::SHUTTING_DOWN;
}
