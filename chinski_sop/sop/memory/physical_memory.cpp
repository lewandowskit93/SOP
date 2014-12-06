//klasa odpowiedzialna za obs³ugê pamiêci fizycznej
#include ".\sop\memory\physical_memory.h"

sop::memory::PhysicalMemory::PhysicalMemory(uint16_t storage_size,uint8_t frame_size)
{
  this->storage_size=storage_size;
  this->frame_size=frame_size;
  storage = boost::shared_ptr< uint8_t[]>(new uint8_t[storage_size]);
  frame_table = boost::shared_ptr< frame[]>(new frame[storage_size/frame_size]);
  this->number_of_frames=storage_size/frame_size;
  setNubmerOfFreeFrames(getNumberOfFrames());
  setNumberOfNotFreeFrames(getNumberOfFrames() - getNumberOfFreeFrames());
  this->guardian_of_free_frames=(uint8_t)ceil((float)getNumberOfFrames()*0.1);
  for(int i=0;i<getNumberOfFrames();++i)//< czy <= ??
  {
    list_of_free_frames.push_back(i);
  }
}
uint8_t sop::memory::PhysicalMemory::getFrameSize()
{
  return frame_size;
}
uint16_t sop::memory::PhysicalMemory::getStorageSize()
{
  return storage_size;
}
uint8_t* sop::memory::PhysicalMemory::getStorage()
{
  return storage.get();
}
uint16_t sop::memory::PhysicalMemory::getNumberOfFrames()
{
  return number_of_frames;
}
uint16_t sop::memory::PhysicalMemory::getNumberOfFreeFrames()
{
  return number_of_free_frames;
}
uint16_t sop::memory::PhysicalMemory::getNumberOfNotFreeFrames()
{
  return number_of_not_free_frames;
}
void sop::memory::PhysicalMemory::setNubmerOfFreeFrames(uint16_t change)
{
  this->number_of_free_frames=change;
}
void sop::memory::PhysicalMemory::setNumberOfNotFreeFrames(uint16_t change)

{
  this->number_of_not_free_frames=change;
}
uint8_t sop::memory::PhysicalMemory::getGuardianLevelofFreeFrames()
{
  return guardian_of_free_frames;
}
bool sop::memory::PhysicalMemory::isEnoughFrames(uint8_t pages_needed)
{
  if((getNumberOfFreeFrames()-pages_needed)>getGuardianLevelofFreeFrames())
    return true;
  else return false;
}
void sop::memory::PhysicalMemory::setFrame(uint16_t pid,uint8_t page_number,uint16_t frame_number)
{
  this->frame_table[frame_number].page_number=page_number;
  this->frame_table[frame_number].pid=pid;
}
sop::memory::frame* sop::memory::PhysicalMemory::getFrame(uint16_t frame_number)
{
  return &frame_table[frame_number];
}
void sop::memory::PhysicalMemory::getFreeFrames(uint8_t pages_needed,sop::memory::LogicalMemory* table_of_pages,uint16_t pid)
{
  uint8_t victim_page=0;//zmienna pomocnicza, zawiera w sobie nr strony(procesu) ktora ma byc wymieciona na swapa
  while(isEnoughFrames(pages_needed)!=true)
  {
  
    //jesli poziom wolnych ramek spadnie poni¿ej poziomu krytycznego(guaridian...) wykonaj:
    //sprawdzenei czy jest miejsce w swapie na wymiecenie strony,jesli nie FATAL ERROR,jeœli tak:
    // find(this->frame_table[this->assigned_frames_queue.front()].pid); //funkcja find ma zwrocic tabele stron/proces ktorego ramka jest na 1. miesjscu w kolejce, na podstawie PID
    //majac proces/jego tabele stron szukamy tej strony, ktora ma ta ramke:
   // for(int i=0;i<tabelaProcesu.getPageTableSize();++i)
   // {
   //   if(tabelaProcesu->getPage(i)->frame_number == this->assigned_frames_queue.front())
   //     victim_page=i;
   // } 
    //przerzucenie zawartosci ramki do swapa, pidu procesu, 
    //zawartosc_ramki_do_swapa(assigned_frames_queue.front()
    //nr ramki tej strony ustawic na ramke ze swapa, valid na 0(czyli invalid)
    //this->assigned_frames_queue.front(); wyczyscic ta ramke,tabela ramek:pid=0, page_number=0;
    this->setNubmerOfFreeFrames(this->getNumberOfFreeFrames()+1);//zwiêkszenie liczby wolnych ramek
    this->setNumberOfNotFreeFrames(this->getNumberOfFrames() - this->getNumberOfFreeFrames());//zmniejszenie liczby zajêtych ramek
    this->list_of_free_frames.push_front(this->assigned_frames_queue.front());//wrzucenie ramki, ktorej strona zosta³a wymieciona na liste wolnych ramek
    this->assigned_frames_queue.pop();//wyrzucenie ramkie, której strona zosta³a wymieciona z kolejki zaalokowanych ramek
  }
  
    //jeœli poziom wolnych ramek jest odpowiedni po zabraniu ramek to wykonaj:
    for(int i=0;i<pages_needed;++i)
    {
      table_of_pages->setPage(this->list_of_free_frames.front(),1,i);//przydzielenie ramki danej stronie i ustawienie valid na 1
      this->setNubmerOfFreeFrames(this->getNumberOfFreeFrames()-1);//zmniejsza liczbê wolnych ramek
      this->setNumberOfNotFreeFrames(this->getNumberOfFrames() - this->getNumberOfFreeFrames());//zwiêksza liczbê zajêtych ramek
      this->setFrame(pid,i,this->list_of_free_frames.front());//przypisanie strony danej ramce, informacje o tym zapisane w tabeli ramek
      this->assigned_frames_queue.push(this->list_of_free_frames.front());//zapisuje w kolejce która ramka zosta³a przypisana
      this->list_of_free_frames.pop_front();//usuwa przydzielon¹ ramkê z listy wolnych ramek
    }
      
}


