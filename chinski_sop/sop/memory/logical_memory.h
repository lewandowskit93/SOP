#ifndef _SOP_MEMORY_LOGICAL_MEMORY_H_
#define _SOP_MEMORY_LOGICAL_MEMORY_H_

#include <stdint.h>
#include <boost\shared_ptr.hpp>
#include ".\sop\logger\logger.h"
namespace sop
{
  namespace memory
  {
    struct page //struktura strony
    {
      uint16_t frame_number;//nr ramki do jakej przynale�y stron
      char valid_invalid;//czy strona w pami�ci czy nie, 0 - invalid, 1 - valid
    };
    class LogicalMemory
    {
    public:	
      explicit LogicalMemory(uint8_t number_of_page,sop::logger::Logger* log);//konstruktor tworz�cy tablic� stron o podanej liczbie stron
      ~LogicalMemory();//destruktor, stworzono dla mo�liwo�ci zapisania do loggera ze co� zosta�o zniszczone
      void setPageTableSize(uint8_t size);//ustawia liczb� stron
      uint8_t getPageTableSize();//zwraca liczb� stron
      page* getPage(uint8_t page_nr);//zwraca wskazan� stron�
      void setPage(uint8_t frame_nr,char val_inv,uint8_t page_nr);//ustawia poszczeg�lnej strony i jej parametr�w
      char getBitValidInvalid(uint8_t page_nr);//zwraca wartosc bitu valid/invalid
      uint16_t getFrameNr(uint8_t page_nr);//zwraca nr ramki ze struktury strona
      void setBitBalidInvalid(uint8_t page_nr, char bit);
    protected:

    private:
      uint8_t page_table_size; //zmienna odpowiadaj�ca za liczb�  stron
      boost::shared_ptr<page[]> page_table;//tabela stron, rekord�w tyle ile ramek
      sop::logger::Logger* loggg; 
    };
    //page_table = boost::shared_ptr<page[]>(new page[page_table_size]); w konruktorze zrobic

  }
}



#endif // _SOP_MEMORY_LOGICAL_MEMORY_H_