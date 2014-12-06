//klasa odpowiedzialna za obs³ugê pamiêci fizycznej
#include ".\sop\memory\physical_memory.h"

sop::memory::PhysicalMemory::PhysicalMemory(int16_t storage_size,int8_t frame_size)
{
  this->storage_size=storage_size;
  this->frame_size=frame_size;
  storage = boost::shared_ptr< int8_t[]>(new int8_t[storage_size]);
  frame_table = boost::shared_ptr< frame[]>(new frame[storage_size/frame_size]);
  this->number_of_frames=storage_size/frame_size;
  setNubmerOfFreeFrames(getNumberOfFrames());
  setNumberOfNotFreeFrames(getNumberOfFrames() - getNumberOfFreeFrames());
}
int8_t sop::memory::PhysicalMemory::getFrameSize()
{
  return frame_size;
}
int16_t sop::memory::PhysicalMemory::getStorageSize()
{
  return storage_size;
}
int8_t* sop::memory::PhysicalMemory::getStorage()
{
  return storage.get();
}
int8_t sop::memory::PhysicalMemory::getNumberOfFrames()
{
  return number_of_frames;
}
int8_t sop::memory::PhysicalMemory::getNumberOfFreeFrames()
{
  return number_of_free_frames;
}
int8_t sop::memory::PhysicalMemory::getNumberOfNotFreeFrames()
{
  return number_of_not_free_frames;
}
void sop::memory::PhysicalMemory::setNubmerOfFreeFrames(int8_t change)
{
  this->number_of_free_frames=change;
}
void sop::memory::PhysicalMemory::setNumberOfNotFreeFrames(int8_t change)

{
  this->number_of_not_free_frames=change;
}

