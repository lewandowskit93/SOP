
#include ".\sop\memory\swap.h"
sop::memory::SwapFile::SwapFile(uint16_t swap_size,uint8_t swap_frame_size,sop::logger::Logger* log):
  loggg(log)
{
  this->swap_size=swap_size;
  this->swap_frame_size=swap_frame_size;
  swap_file = boost::shared_ptr< uint8_t[]>(new uint8_t[swap_size]);
  swap_frame_table = boost::shared_ptr< swapFrame[]>(new swapFrame[swap_size/swap_frame_size]);
  this->number_of_swap_frames=swap_size/swap_frame_size;
  setNubmerOfFreeSwapFrames(getNumberOfSwapFrames());
  for(int i=0;i<getNumberOfSwapFrames();++i)
  {
    list_of_free__swap_frames.push_back(i);
  }
}

sop::memory::SwapFile::~SwapFile()
{

}

uint8_t sop::memory::SwapFile::getSwapFrameSize()
{
  return this->swap_frame_size;
}

uint8_t* sop::memory::SwapFile::getSwap()
{
  return swap_file.get();
}

uint16_t sop::memory::SwapFile::getNumberOfSwapFrames()
{
  return this->number_of_swap_frames;
}

uint16_t sop::memory::SwapFile::getNumberOfFreeSwapFrames()
{
  return this->number_of_free_swap_frames;
}

void sop::memory::SwapFile::setNubmerOfFreeSwapFrames(uint16_t change)
{
  this->number_of_free_swap_frames=change;
}

sop::memory::swapFrame* sop::memory::SwapFile::getSwapFrame(uint16_t swap_frame_number)
{
  return &swap_frame_table[swap_frame_number];
}

void sop::memory::SwapFile::setSwapFrame(uint16_t pid,uint8_t page_number,uint16_t swap_frame_number)
{
  this->swap_frame_table[swap_frame_number].page_number=page_number;
  this->swap_frame_table[swap_frame_number].pid=pid;
}

bool sop::memory::SwapFile::isThereAnyFrame()
{
  if(getNumberOfFreeSwapFrames()>1) 
    return true;
  else return false;
}

bool sop::memory::SwapFile::getIsThereAnyFrameValue()
{
  return isThereAnyFrame();
}

uint8_t sop::memory::SwapFile::getFreeFrame()
{
  return this->list_of_free__swap_frames.front();
}

void sop::memory::SwapFile::popFrontListOfFreeSwapFrames()
{
  this->list_of_free__swap_frames.pop_front();
}

void::sop::memory::SwapFile::pushEndListOfFreeSwapFrames(uint16_t frame_nr)
{
  this->list_of_free__swap_frames.push_back(frame_nr);
}

uint16_t sop::memory::SwapFile::getSwapSize()
{
  return this->swap_size;
}

std::list <uint16_t> sop::memory::SwapFile::getListForFreeSwapFrames()
{
  return this->list_of_free__swap_frames;
}