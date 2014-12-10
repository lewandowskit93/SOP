//klasa odpowiedzialna za obs³ugê pamiêci fizycznej
#include ".\sop\memory\physical_memory.h"

sop::memory::PhysicalMemory::PhysicalMemory(uint16_t storage_size,uint8_t frame_size,sop::logger::Logger* log):
  loggg(log)
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

sop::memory::PhysicalMemory::~PhysicalMemory()
{

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

void sop::memory::PhysicalMemory::popFrontListOfFreeFrames()
{
  this->list_of_free_frames.pop_front();
}

void::sop::memory::PhysicalMemory::pushEndListOfFreeFrames(uint16_t frame_nr)
{
  this->list_of_free_frames.push_back(frame_nr);
}

void::sop::memory::PhysicalMemory::FindAndEraseFromDeque(uint16_t frame_nr)
{
  uint16_t temp=0;//zmienna sluzaca do przeusniecia iteratora tak zeby usunac to co trzeba
  std::deque<uint16_t>::iterator it = this->assigned_frames_deque.begin();//iterator na poczatek kolejki
for (uint16_t i=0; i<this->assigned_frames_deque.size(); ++i)//znalezienie ramki w kolejce i ustawienie tempa
 {
   if(assigned_frames_deque.at(i)==frame_nr)
     break;
   temp=temp+1;
   

 }
 this->assigned_frames_deque.erase (it+temp,it+temp+1);//usuniecie znalezionego elementu z kolejki
}

void sop::memory::PhysicalMemory::swap(SwapFile* file_swap,LogicalMemory* page_table,int8_t victim_page,int8_t frame)
{
  
  //zawartosc_ramki_do_swapa przerzucic(assigned_frames_deque.front()
  for(int i=this->getFrameSize()*frame,j=this->getFrameSize()*file_swap->getFreeFrame();i<this->getFrameSize();++i,++j)
  {
     file_swap->getSwap()[j]=this->getStorage()[i];
   }
   page_table->setPage(file_swap->getFreeFrame(),0,victim_page);//numer ramki w tablicy stron ustawiony na numer ramki ze swapa valid na 0
   file_swap->setSwapFrame(this->getFrame(frame)->pid,victim_page,file_swap->getFreeFrame());//zapisanie informacji o ramce na swapie w tabeli ramek, 
   this->setFrame(0,0,frame);//czyscimy ramke przerzucon¹ na swapa
   file_swap->popFrontListOfFreeSwapFrames();//usuniecie ramki z listy wolnych ramek w swapie
   file_swap->setNubmerOfFreeSwapFrames(file_swap->getNumberOfSwapFrames()-1);//zmniejsza liczbe wolnych ramek w swapie
 }

void sop::memory::PhysicalMemory::getFreeFrames(uint8_t pages_needed,sop::memory::LogicalMemory* table_of_pages,uint16_t pid,SwapFile* file_swap)
{
  uint8_t victim_page=0;//zmienna pomocnicza, zawiera w sobie nr strony(procesu) ktora ma byc wymieciona na swapa
  while(isEnoughFrames(pages_needed)!=true)
  {
  
    //jesli poziom wolnych ramek spadnie poni¿ej poziomu krytycznego(guaridian...) wykonaj:
    if(file_swap->getIsThereAnyFrameValue()==true)//sprawdzenie czy swap nie jest zapchany
    {
   
    // find(this->frame_table[this->assigned_frames_deque.front()].pid); //funkcja find ma zwrocic tabele stron/proces ktorego ramka jest na 1. miesjscu w kolejce, na podstawie PID
    //majac proces/jego tabele stron szukamy tej strony, ktora ma ta ramke:
   // for(int i=0;i<tabelaProcesu.getPageTableSize();++i)
   // {
   //   if(tabelaProcesu->getPage(i)->frame_number == this->assigned_frames_deque.front())
   //     victim_page=i;
   // } 
    swap(file_swap,table_of_pages,victim_page,this->assigned_frames_deque.front());//przerzucenie zawartosci struktury ramka do swapa
    this->setNubmerOfFreeFrames(this->getNumberOfFreeFrames()+1);//zwiêkszenie liczby wolnych ramek
    this->setNumberOfNotFreeFrames(this->getNumberOfFrames() - this->getNumberOfFreeFrames());//zmniejszenie liczby zajêtych ramek
    this->list_of_free_frames.push_front(this->assigned_frames_deque.front());//wrzucenie ramki, ktorej strona zosta³a wymieciona na liste wolnych ramek
    this->assigned_frames_deque.pop_front();//wyrzucenie ramkie, której strona zosta³a wymieciona z kolejki zaalokowanych ramek
    
  }
    //else FATALERROR
     // break;
  }
  
    //jeœli poziom wolnych ramek jest odpowiedni po zabraniu ramek to wykonaj:
    for(int i=0;i<pages_needed;++i)
    {
      ///dobrze zmiana liczby strony?
      table_of_pages->setPage(this->list_of_free_frames.front(),1,i);//przydzielenie ramki danej stronie i ustawienie valid na 1
      this->setNubmerOfFreeFrames(this->getNumberOfFreeFrames()-1);//zmniejsza liczbê wolnych ramek
      this->setNumberOfNotFreeFrames(this->getNumberOfFrames() - this->getNumberOfFreeFrames());//zwiêksza liczbê zajêtych ramek
      this->setFrame(pid,i,this->list_of_free_frames.front());//przypisanie strony danej ramce, informacje o tym zapisane w tabeli ramek
      this->assigned_frames_deque.push_back(this->list_of_free_frames.front());//zapisuje w kolejce która ramka zosta³a przypisana
      this->list_of_free_frames.pop_front();//usuwa przydzielon¹ ramkê z listy wolnych ramek
    }
      
}


