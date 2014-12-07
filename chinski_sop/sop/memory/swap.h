#ifndef _SOP_MEMORY_SWAP_H_
#define _SOP_MEMORY_SWAP_H_

#include <stdint.h>
#include <boost\shared_ptr.hpp>
#include <list>

namespace sop
{
namespace memory
{
  struct swapFrame
  {
    uint16_t pid;
    uint8_t page_number;
  };

  class SwapFile
  {
public:	
  uint8_t getSwapFrameSize();//zwraca rozmiar ramki w swapie,taki sam jak w fizycznej pamieci
  uint8_t* getSwap();//zwraca wskaŸnik na swapa
  SwapFile(uint16_t swap_size,uint8_t swap_frame_size);//kontstruktor tworz¹cy plik wymiany i ustawiajacy liczbê ramek
  uint16_t getNumberOfSwapFrames();//zwraca liczbe ramek w pliku wymainy
  uint16_t getNumberOfFreeSwapFrames();//zwraca liczbê wolnych ramek w pliku wymiany
  void setNubmerOfFreeSwapFrames(uint16_t change);//ustawia liczbê wolnych ramek w pliku wymiany
  swapFrame* getSwapFrame(uint16_t swap_frame_number);//zwraca opis danej ramki z tabeli ramek
  void setSwapFrame(uint16_t pid,uint8_t page_number,uint16_t swap_frame_number);//ustawia rekord w tabeli ramek na podstawie podanych argumentów
  bool getIsThereAnyFrameValue();//zwraca czy swap nie jest zapchany
  uint8_t getFreeFrame();//zwraca pierwsza¹ woln¹ ramkê
  void popFrontListOfFreeSwapFrames();//usuwa ramke bedaca z przodu listy wolnych ramek
  void pushEndListOfFreeSwapFrames(uint16_t frame_nr);//wstawia na koniec lisy nr wolnej ramki
protected:
  
private:
  bool isThereAnyFrame();//funkcja zwracaj¹ca czy swap nie jest zapchany
  uint8_t swap_frame_size;//rozmiar ramki w swapie, czyli tak sam jak ramki fizycznej
  uint16_t swap_size; //zmienna odpowiadaj¹ca za rozmiar pamiêci swapa
  uint16_t number_of_swap_frames;//liczba ramek w swapie
  uint16_t number_of_free_swap_frames;//liczba wolnych ramek w swapie
  boost::shared_ptr<uint8_t[]> swap_file;//w³aœciwy swap file
  boost::shared_ptr<swapFrame[]> swap_frame_table;//tabela ramek dla swapu
  std::list <uint16_t> list_of_free__swap_frames;//lista wolnych ramek w swapie
 
  };

}
}
#endif _SOP_MEMORY_SWAP_H_