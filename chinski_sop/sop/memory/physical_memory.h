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
  int16_t getStorageSize();//zwraca rozmiar pami�ci
  int8_t* getStorage();//zwraca wska�nik na pami�� fizyczn�
  PhysicalMemory(int16_t storage_size,int8_t frame_size);//kontstruktor tworz�cy pami�� i ustawiajacy liczb� ramek
  int8_t getNumberOfFrames();//zwraca liczbe ramek
  int8_t getNumberOfFreeFrames();//zwraca liczb� wolnych ramek
  int8_t getNumberOfNotFreeFrames();//zwraca liczb� zaj�tych ramek
  void setNubmerOfFreeFrames(int8_t change);//ustawia liczb� wolnych ramek
  void setNumberOfNotFreeFrames(int8_t change);//ustawia liczb� zaj�tych ramek
  
protected:

private:
  int8_t frame_size;//rozmiar ramki
  int16_t storage_size; //zmienna odpowiadaj�ca za rozmiar pami�ci
  int8_t number_of_frames;//liczba ramek
  int8_t number_of_free_frames;//liczba wolnych ramek
  int8_t number_of_not_free_frames;//liczba zaj�tych ramek
  boost::shared_ptr<int8_t[]> storage;//w�a�ciwa pami�� fizyczna
  boost::shared_ptr<frame[]> frame_table;//tabela ramek
 
};

}

}


#endif //_SOP_MEMORY_PHYSICAL_MEMORY_H_