#include "./sop/processor/executive_unit.h"
#include "./sop/processor/scheduler.h"
#include "./sop/processor/interpreter.h"
#include "./sop/system/shell.h"
#include <iostream>
sop::processor::ExecutiveUnit::ExecutiveUnit(sop::logger::Logger* logger):
  logger(logger),
  _standardQuantTime(3),
  _quantTimeLeft(_standardQuantTime)
  {
    this->logger->logProcessor(3,"Initializing processing unit.");
    this->logger->logProcessor(3,"Looking for errors...");
    scheduler.getHighestPriorityProcess();
    this->logger->logProcessor(3,"Initialization done.");
  }

sop::processor::ExecutiveUnit::~ExecutiveUnit(){
}

short sop::processor::ExecutiveUnit::getQuantTimeLeft()
{
  return _quantTimeLeft;
}
void sop::processor::ExecutiveUnit::resetQuantTime()
{
  _quantTimeLeft = _standardQuantTime;
}
void sop::processor::ExecutiveUnit::processorTick()
{

  _quantTimeLeft--;
}

void sop::processor::ExecutiveUnit::cH_showQuantTimeLeft(const std::vector<const std::string> & params)
{
  this->logger->logProcessor(3, "Getting quant time left");
  if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<getQuantTimeLeft()<<std::endl;


}


