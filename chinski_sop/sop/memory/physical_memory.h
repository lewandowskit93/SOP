#ifndef _SOP_MEMORY_PHYSICAL_MEMORY_H_
#define _SOP_MEMORY_PHYSICAL_MEMORY_H_

#include <stdint.h>
#include <boost\shared_ptr.hpp>
#include <list>
#include <queue>
#include "logical_memory.h"
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
  uint8_t getGuardianLevelofFreeFrames();//zwraca krytyczn¹ liczbê wolnych ramek
  void getFreeFrames(uint8_t pages_needed,sop::memory::LogicalMemory* table_of_pages,uint16_t pid);//funkcja odpowiedzialna za przydzielenie ramek stronom, na podstawie liczby poz¹danych stron
  void setFrame(uint16_t pid,uint8_t page_number,uint16_t frame_number);//ustawia rekord w tabeli ramek na podstawie podanych argumentów
  frame* getFrame(uint16_t frame_number);//zwraca opis danej ramki z tabeli ramek
protected:
  
private:
  bool isEnoughFrames(uint8_t pages_needed);//funkcja zwracaj¹ca czy jest wystarczaj¹co ramek, aby mo¿na by³o je przydzieliæ "z marszu" stronom, czy trzeba wykonaæ swapa
  uint8_t frame_size;//rozmiar ramki
  uint16_t storage_size; //zmienna odpowiadaj¹ca za rozmiar pamiêci
  uint16_t number_of_frames;//liczba ramek
  uint16_t number_of_free_frames;//liczba wolnych ramek
  uint16_t number_of_not_free_frames;//liczba zajêtych ramek
  uint8_t guardian_of_free_frames;
  boost::shared_ptr<uint8_t[]> storage;//w³aœciwa pamiêæ fizyczna
  boost::shared_ptr<frame[]> frame_table;//tabela ramek
  std::list <uint16_t> list_of_free_frames;//lista wolnych ramek
  std::queue <uint16_t>assigned_frames_queue;//kolejka FIFO przydzielanych ramek, potrzebna przy wymianie stron(algorytm FIFO)
 
};

}

}


#endif //_SOP_MEMORY_PHYSICAL_MEMORY_H_