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
  uint16_t frame_number;//nr ramki do jakej przynale¿y stron
  char valid_invalid;//czy strona w pamiêci czy nie, 0 - invalid, 1 - valid
};
class LogicalMemory
{
public:	
  explicit LogicalMemory(uint8_t number_of_page);//konstruktor tworz¹cy tablicê stron o podanej liczbie stron
  ~LogicalMemory();//destruktor, stworzono dla mo¿liwoœci zapisania do loggera ze coœ zosta³o zniszczone
  void setPageTableSize(uint8_t size);//ustawia liczbê stron
  uint8_t getPageTableSize();//zwraca liczbê stron
  page* getPage(uint8_t page_nr);//zwraca wskazan¹ stronê
  void setPage(uint8_t frame_nr,char val_inv,uint8_t page_nr);//ustawia poszczególnej strony i jej parametrów
  char getBitValidInvalid(uint8_t page_nr);//zwraca wartosc bitu valid/invalid
  uint16_t getFrameNr(uint8_t page_nr);//zwraca nr ramki ze struktury strona
  
protected:

private:
	uint8_t page_table_size; //zmienna odpowiadaj¹ca za liczbê  stron
  boost::shared_ptr<page[]> page_table;//tabela stron, rekordów tyle ile ramek
};
//page_table = boost::shared_ptr<page[]>(new page[page_table_size]); w konruktorze zrobic

}
}



#endif // _SOP_MEMORY_LOGICAL_MEMORY_H_