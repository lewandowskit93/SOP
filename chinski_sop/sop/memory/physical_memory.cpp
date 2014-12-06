//klasa odpowiedzialna za obs³ugê pamiêci fizycznej
#include ".\sop\memory\physical_memory.h"

sop::memory::PhysicalMemory::PhysicalMemory(uint16_t storage_size,uint8_t frame_size)
{
  this->storage_size=storage_size;
  this->frame_size=frame_size;
  storage = boost::shared_ptr< uint8_t[]>(new uint8_t[storage_size]);
  frame_table = boost::shared_ptr< frame[]>(new frame[storage_size/frame_size]);
  this->number_of_frames=storage_size/frame_size;
  setNubmerOfFreeFrames(getNumberOfFrames());
  setNumberOfNotFreeFrames(getNumberOfFrames() - getNumberOfFreeFrames());
}
uint8_t sop::memory::PhysicalMemory::getFrameSize()
{
  return frame_size;
}
uint16_t sop::memory::PhysicalMemory::getStorageSize()
{
  return storage_size;
}
uint8_t* sop::memory::PhysicalMemory::getStorage()
{
  return storage.get();
}
uint16_t sop::memory::PhysicalMemory::getNumberOfFrames()
{
  return number_of_frames;
}
uint16_t sop::memory::PhysicalMemory::getNumberOfFreeFrames()
{
  return number_of_free_frames;
}
uint16_t sop::memory::PhysicalMemory::getNumberOfNotFreeFrames()
{
  return number_of_not_free_frames;
}
void sop::memory::PhysicalMemory::setNubmerOfFreeFrames(uint16_t change)
{
  this->number_of_free_frames=change;
}
void sop::memory::PhysicalMemory::setNumberOfNotFreeFrames(uint16_t change)

{
  this->number_of_not_free_frames=change;
}

