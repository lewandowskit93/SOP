#include "./sop/processor/scheduler.h"
uint8_t sop::processor::Scheduler::getUserPriority(sop::process::Process *p)
{
  // funkcja Tomalska sop::user::GetUserPriority(p.getUID);
  //random for nw
  uint8_t priority = rand() % 10 - 5;
  return priority;
}
void sop::processor::Scheduler::calculatePriority()
{
  //for now it's pointless, I guess that the priorities will not be changed
  //I'll think on it..
}

void sop::processor::Scheduler::addProcess(sop::process::Process *p, sop::processor::Scheduler *sched)
{
  sched->addToActiveTaskArray(p);
}

sop::process::Process *sop::processor::Scheduler::getHighestPriorityProcess()
{
  if (firstIsActive())
  {
    for (int i = 0 ; i < 10 ; i++)
    {
      if (_first_task_array[i].size()>0)
      {
        return _first_task_array[i].back();
      }
    }
    eraChange();
  }
  else if (secondIsActive())
  {
   for (int i = 0 ; i < 10 ; i++)
    {
      if (_second_task_array[i].size()>0)
      {
        return _second_task_array[i].back();
      }
    }
   eraChange();
  }
  return nullptr;
}

void sop::processor::Scheduler::addToUnactiveTaskArray(sop::process::Process *p)
{
  uint8_t priority = getUserPriority(p)+5;
  if (!firstIsActive())
  {
    _first_task_array[priority].push(p);
  }
  else if (!secondIsActive())
  {
    _second_task_array[priority].push(p);
  }

}

void sop::processor::Scheduler::addToActiveTaskArray(sop::process::Process *p)
{
  uint8_t priority = getUserPriority(p)+5;
  if (firstIsActive())
  {
    _first_task_array[priority].push(p);
  }
  else if (secondIsActive())
  {
    _second_task_array[priority].push(p);
  }
}

void sop::processor::Scheduler::eraChange()
{
  if (_isFirstActive)
  {
    clearTaskArray();
    _isFirstActive = false;
    _isSecondActive = true;
  }
  else if (_isSecondActive){
    clearTaskArray();
    _isFirstActive = true;
    _isSecondActive = false;
  }
}

void sop::processor::Scheduler::clearTaskArray()
{
  if (_isSecondActive)
    _second_task_array.clear();
  else if (_isFirstActive)
    _first_task_array.clear();
}
bool sop::processor::Scheduler::firstIsActive()
{
  if (_isFirstActive) return true;
  else return false;
}

bool sop::processor::Scheduler::secondIsActive()
{
  if (_isSecondActive) return true;
  else return false;
}

sop::processor::Scheduler::Scheduler():
  _isFirstActive(true),
  _isSecondActive(false)
  {
    for (uint8_t i = 0 ; i < 10 ; i++)
    {
      _first_task_array.push_back(std::queue<sop::process::Process*>());
    }
  }

sop::processor::Scheduler::~Scheduler()
{}