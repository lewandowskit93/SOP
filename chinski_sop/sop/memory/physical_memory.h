#ifndef _SOP_MEMORY_PHYSICAL_MEMORY_H_
#define _SOP_MEMORY_PHYSICAL_MEMORY_H_

#include <stdint.h>
#include <boost\shared_ptr.hpp>
namespace sop
{
namespace memory
{
  struct frame
  {
    uint16_t pid;
    uint8_t page_number;
    
  };

class PhysicalMemory
{
public:	
  uint8_t getFrameSize();//zwraca rozmiar ramki
  uint16_t getStorageSize();//zwraca rozmiar pamiêci
  uint8_t* getStorage();//zwraca wskaŸnik na pamiêæ fizyczn¹
  PhysicalMemory(uint16_t storage_size,uint8_t frame_size);//kontstruktor tworz¹cy pamiêæ i ustawiajacy liczbê ramek
  uint16_t getNumberOfFrames();//zwraca liczbe ramek
  uint16_t getNumberOfFreeFrames();//zwraca liczbê wolnych ramek
  uint16_t getNumberOfNotFreeFrames();//zwraca liczbê zajêtych ramek
  void setNubmerOfFreeFrames(uint16_t change);//ustawia liczbê wolnych ramek
  void setNumberOfNotFreeFrames(uint16_t change);//ustawia liczbê zajêtych ramek
  
protected:

private:
  uint8_t frame_size;//rozmiar ramki
  int16_t storage_size; //zmienna odpowiadaj¹ca za rozmiar pamiêci
  uint16_t number_of_frames;//liczba ramek
  uint16_t number_of_free_frames;//liczba wolnych ramek
  uint16_t number_of_not_free_frames;//liczba zajêtych ramek
  boost::shared_ptr<uint8_t[]> storage;//w³aœciwa pamiêæ fizyczna
  boost::shared_ptr<frame[]> frame_table;//tabela ramek
 
};

}

}


#endif //_SOP_MEMORY_PHYSICAL_MEMORY_H_