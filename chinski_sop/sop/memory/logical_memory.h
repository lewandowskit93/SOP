#ifndef _SOP_MEMORY_LOGICAL_MEMORY_H_
#define _SOP_MEMORY_LOGICAL_MEMORY_H_

#include <stdint.h>
#include <boost\shared_ptr.hpp>

namespace sop
{
namespace memory
{
struct page //struktura strony
{
  int8_t frame_number;//nr ramki do jakej przynale¿y stron
  char valid_invalid;//czy strona w pamiêci czy nie
};
class LogicalMemory
{
public:	
  explicit LogicalMemory(int8_t number_of_page);//konstruktor tworz¹cy tablicê stron o podanej liczbie stron
  void setPageTableSize(int8_t size);//ustawia liczbê stron
  uint8_t getPageTableSize();//zwraca liczbê stron
  page* getPage(int8_t page_nr);//zwraca wskazan¹ stronê
  void setPage(int8_t frame_nr,char val_inv,int8_t page_nr);//ustawia poszczególnej strony i jej parametrów
  
protected:

private:
	int8_t page_table_size; //zmienna odpowiadaj¹ca za liczbê  stron
  boost::shared_ptr<page[]> page_table;//tabela stron, rekordów tyle ile ramek
};
//page_table = boost::shared_ptr<page[]>(new page[page_table_size]); w konruktorze zrobic

}
}



#endif // _SOP_MEMORY_LOGICAL_MEMORY_H_