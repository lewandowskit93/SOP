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
  uint16_t getStorageSize();//zwraca rozmiar pami�ci
  uint8_t* getStorage();//zwraca wska�nik na pami�� fizyczn�
  PhysicalMemory(uint16_t storage_size,uint8_t frame_size);//kontstruktor tworz�cy pami�� i ustawiajacy liczb� ramek
  uint16_t getNumberOfFrames();//zwraca liczbe ramek
  uint16_t getNumberOfFreeFrames();//zwraca liczb� wolnych ramek
  uint16_t getNumberOfNotFreeFrames();//zwraca liczb� zaj�tych ramek
  void setNubmerOfFreeFrames(uint16_t change);//ustawia liczb� wolnych ramek
  void setNumberOfNotFreeFrames(uint16_t change);//ustawia liczb� zaj�tych ramek
  
protected:

private:
  uint8_t frame_size;//rozmiar ramki
  int16_t storage_size; //zmienna odpowiadaj�ca za rozmiar pami�ci
  uint16_t number_of_frames;//liczba ramek
  uint16_t number_of_free_frames;//liczba wolnych ramek
  uint16_t number_of_not_free_frames;//liczba zaj�tych ramek
  boost::shared_ptr<uint8_t[]> storage;//w�a�ciwa pami�� fizyczna
  boost::shared_ptr<frame[]> frame_table;//tabela ramek
 
};

}

}


#endif //_SOP_MEMORY_PHYSICAL_MEMORY_H_