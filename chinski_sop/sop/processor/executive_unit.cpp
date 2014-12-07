#include "./sop/processor/executive_unit.h"
#include "./sop/processor/scheduler.h"
#include "./sop/processor/interpreter.h"
sop::processor::ExecutiveUnit::ExecutiveUnit():
  _standardQuantTime(3),
  _quantTimeLeft(_standardQuantTime)
  {}

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