//klasa odpowiedzialna za obs³ugê pamiêci logicznej
#include ".\sop\memory\logical_memory.h"

sop::memory::LogicalMemory::LogicalMemory(uint8_t number_of_page)
{
  setPageTableSize(number_of_page);
  page_table = boost::shared_ptr<page[]>(new page[page_table_size]);
  
}

void sop::memory::LogicalMemory::setPageTableSize(uint8_t size)
{
  this->page_table_size=size;
}

uint8_t sop::memory::LogicalMemory::getPageTableSize()
{
  return page_table_size;
}

sop::memory::page* sop::memory::LogicalMemory::getPage(uint8_t page_nr)
{
  return &page_table[page_nr];
}

void sop::memory::LogicalMemory::setPage(uint8_t frame_nr,char val_inv,uint8_t page_nr)
{
  this->page_table[page_nr].frame_number=frame_nr;
  this->page_table[page_nr].valid_invalid=val_inv;
}



