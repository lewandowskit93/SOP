#include ".\sop\processor\module.h"
#include ".\sop\system\kernel.h"
#include ".\sop\processor\executive_unit.h"
sop::processor::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel)
{
  this->ExUnit = new ExecutiveUnit(kernel->getLogger());
}

sop::processor::Module::~Module()
{

}

std::string sop::processor::Module::getClassName() const
{
  return "sop::processor::Module";
}

void sop::processor::Module::initialize()
{
  
  sop::system::Shell* shell = this->_kernel->getShell();
  sop::logger::Logger* logger = this->_kernel->getLogger();
  shell->registerCommand("sqtleft",&ExecutiveUnit::cH_showQuantTimeLeft,(this->ExUnit));
  shell->registerCommand("sacttask",&ExecutiveUnit::cH_showActiveTaskQueue,(this->ExUnit));
  shell->registerCommand("suacttask",&ExecutiveUnit::cH_showUnactiveTaskQueue,(this->ExUnit));
  shell->registerCommand("procstate",&ExecutiveUnit::cH_showActualProcessorState,(this->ExUnit));
  shell->registerCommand("fulltick",&ExecutiveUnit::cH_fullTick,(this->ExUnit));
  //shell->registerCommand("aprocstate",&ExecutiveUnit::cH_showAnyProcessorState,(this->ExUnit));
}