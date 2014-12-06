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
    int16_t pid;
    int8_t page_number;

  };

class PhysicalMemory
{
public:	
  int8_t getFrameSize();//zwraca rozmiar ramki
  int16_t getStorageSize();//zwraca rozmiar pamiêci
  int8_t* getStorage();//zwraca wskaŸnik na pamiêæ fizyczn¹
  PhysicalMemory(int16_t storage_size,int8_t frame_size);//kontstruktor tworz¹cy pamiêæ i ustawiajacy liczbê ramek
  int8_t getNumberOfFrames();//zwraca liczbe ramek
  int8_t getNumberOfFreeFrames();//zwraca liczbê wolnych ramek
  int8_t getNumberOfNotFreeFrames();//zwraca liczbê zajêtych ramek
  void setNubmerOfFreeFrames(int8_t change);//ustawia liczbê wolnych ramek
  void setNumberOfNotFreeFrames(int8_t change);//ustawia liczbê zajêtych ramek
  
protected:

private:
  int8_t frame_size;//rozmiar ramki
  int16_t storage_size; //zmienna odpowiadaj¹ca za rozmiar pamiêci
  int8_t number_of_frames;//liczba ramek
  int8_t number_of_free_frames;//liczba wolnych ramek
  int8_t number_of_not_free_frames;//liczba zajêtych ramek
  boost::shared_ptr<int8_t[]> storage;//w³aœciwa pamiêæ fizyczna
  boost::shared_ptr<frame[]> frame_table;//tabela ramek
 
};

}

}


#endif //_SOP_MEMORY_PHYSICAL_MEMORY_H_