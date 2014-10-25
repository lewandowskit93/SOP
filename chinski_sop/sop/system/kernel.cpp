#include ".\sop\system\kernel.h"
#include ".\sop\logger\console_logger.h"
#include ".\sop\system\exceptions\module_exceptions.h"
#include ".\sop\system\exceptions\kernel_exceptions.h"

sop::system::Kernel::Kernel():
  sop::Object(),
  _logger(new sop::logger::ConsoleLogger),
  _shell(this),
  _users_module(this),
  _files_module(this),
  _processes_module(this),
  _memory_module(this),
  _processor_module(this),
  _kernel_state(State::PRE_INIT)
{
  _modules.reserve(5);
  _modules.push_back(&_users_module);
  _modules.push_back(&_files_module);
  _modules.push_back(&_processes_module);
  _modules.push_back(&_memory_module);
  _modules.push_back(&_processor_module);
}

sop::system::Kernel::Kernel(uint16_t logging_level):
  sop::Object(),
  _logger(new sop::logger::ConsoleLogger(logging_level)),
  _shell(this),
  _users_module(this),
  _files_module(this),
  _processes_module(this),
  _memory_module(this),
  _processor_module(this),
  _kernel_state(State::PRE_INIT)
{
  _modules.reserve(5);
  _modules.push_back(&_users_module);
  _modules.push_back(&_files_module);
  _modules.push_back(&_processes_module);
  _modules.push_back(&_memory_module);
  _modules.push_back(&_processor_module);
}

sop::system::Kernel::~Kernel()
{
}

std::string sop::system::Kernel::getClassName() const
{
  return "sop::system::Kernel";
}

void sop::system::Kernel::initialize()
{
  _logger->logKernel(sop::logger::Logger::Level::INFO, "Initializing kernel.");
  if(_kernel_state==State::PRE_INIT)
  {
    _logger->logKernel(sop::logger::Logger::Level::INFO, "Initializing modules.");
    for(std::vector<sop::system::Module>::size_type i=0; i<_modules.size(); ++i)
    {
      _logger->logKernel(sop::logger::Logger::Level::INFO, "Initializing module: "+_modules[i]->getClassName());
      try
      {
        _modules[i]->initializeModule();
      }
      catch(const sop::system::exceptions::ModuleInitializationException & exception)
      {
        _logger->logKernel(sop::logger::Logger::Level::SEVERE, "Module: "+_modules[i]->getClassName()+" initialization has failed.");
        throw sop::system::exceptions::KernelInitializationException("Module: "+_modules[i]->getClassName()+" initialization has failed.");
      }
      _logger->logKernel(sop::logger::Logger::Level::INFO, "Module: "+_modules[i]->getClassName() + " initialized.");
    }
    _kernel_state=INITIALIZED;
    _logger->logKernel(sop::logger::Logger::Level::INFO, "Kernel initialized.");
  }
  else
  {
    _logger->logKernel(sop::logger::Logger::Level::SEVERE, "Invalid kernel state.");
    throw sop::system::exceptions::KernelInitializationException("Invalid kernel state.");
  }
}

void sop::system::Kernel::run()
{
  _logger->logKernel(sop::logger::Logger::Level::INFO, "Starting kernel.");
  if(_kernel_state==State::INITIALIZED)
  {
    _kernel_state=State::RUNNING;
    _logger->logKernel(sop::logger::Logger::Level::INFO, "Kernel is running.");
    while(!isShuttingDown())
    {
      _logger->logKernel(sop::logger::Logger::Level::INFO, "Executing next shell step.");
      _shell.step();
    }
    _logger->logKernel(sop::logger::Logger::Level::INFO, "Shutting down the kernel.");
  }
  else
  {
    _logger->logKernel(sop::logger::Logger::Level::SEVERE, "Invalid kernel state.");
    throw sop::system::exceptions::KernelRunException("Invalid kernel state.");
  }
}

const sop::logger::Logger * sop::system::Kernel::getLogger() const
{
  return _logger.get();
}

sop::logger::Logger * sop::system::Kernel::getLogger()
{
  return _logger.get();
}

const sop::system::Shell * sop::system::Kernel::getShell() const
{
  return &_shell;
}

sop::system::Shell * sop::system::Kernel::getShell()
{
  return &_shell;
}

sop::users::Module * sop::system::Kernel::getUsersModule()
{
   return &_users_module;
}

const sop::users::Module * sop::system::Kernel::getUsersModule() const
{
  return &_users_module;
}

sop::files::Module * sop::system::Kernel::getFilesModule()
{
  return &_files_module;
}

const sop::files::Module * sop::system::Kernel::getFilesModule() const
{
  return &_files_module;
}

sop::processes::Module * sop::system::Kernel::getProcessesModule()
{
  return &_processes_module;
}

const sop::processes::Module * sop::system::Kernel::getProcessesModule() const
{
  return &_processes_module;
}

sop::memory::Module * sop::system::Kernel::getMemoryModule()
{
  return &_memory_module;
}

const sop::memory::Module * sop::system::Kernel::getMemoryModule() const
{
  return &_memory_module;
}

sop::processor::Module * sop::system::Kernel::getProcessorModule()
{
  return &_processor_module;
}

const sop::processor::Module * sop::system::Kernel::getProcessorModule() const
{
  return &_processor_module;
}

std::vector<sop::system::Module*> sop::system::Kernel::getModules()
{
  return _modules;
}

std::vector<const sop::system::Module*> sop::system::Kernel::getModules() const
{
  std::vector<const sop::system::Module*> modules;
  for(std::vector<sop::system::Module>::size_type i=0; i<_modules.size(); ++i)
  {
    modules.push_back(_modules[i]);
  }
  return modules;
}

void sop::system::Kernel::shutdown() const
{
  _logger->logKernel(sop::logger::Logger::Level::INFO, "Initializing shutdown procedure.");
  if(_kernel_state==State::RUNNING)
  {
    _kernel_state=State::SHUTTING_DOWN;
    _logger->logKernel(sop::logger::Logger::Level::INFO, "Shutdown procedure initialized.");
  }
  else if(_kernel_state!=State::SHUTTING_DOWN)
  {
    _logger->logKernel(sop::logger::Logger::Level::SEVERE, "Invalid kernel state.");
    throw sop::system::exceptions::KernelShutdownException("Invalid kernel state.");
  }
  else{
    _logger->logKernel(sop::logger::Logger::Level::INFO, "Kernel is already shutting down.");
  }
}

bool sop::system::Kernel::isShuttingDown() const
{
  return _kernel_state==State::SHUTTING_DOWN;
}
