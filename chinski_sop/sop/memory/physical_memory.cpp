//klasa odpowiedzialna za obs³ugê pamiêci fizycznej
#include ".\sop\memory\physical_memory.h"
#include ".\sop\string_converter.h"

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
//  this->guardian_of_free_frames=(uint8_t)ceil((float)getNumberOfFrames()*0.1);
  for(int i=0;i<getNumberOfFrames();++i)
  //for(int i=0;i<getNumberOfFrames()-getGuardianLevelofFreeFrames();++i)//wersja z wartoscia krytyczna
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
/*
uint8_t sop::memory::PhysicalMemory::getGuardianLevelofFreeFrames()
{
  return guardian_of_free_frames;
}
*/
bool sop::memory::PhysicalMemory::isEnoughFrames(uint8_t pages_needed)
{
  if((getNumberOfFreeFrames()-pages_needed)>=0)//getGuardianLevelofFreeFrames())
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

std::list <uint16_t> sop::memory::PhysicalMemory::getListForFreeFrames()
{
  return this->list_of_free_frames;
}

/*
std::deque <uint16_t> sop::memory::PhysicalMemory::getDequeFrames()
{
  return this->assigned_frames_deque;
}
*/

/*

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
*/
void sop::memory::PhysicalMemory::swap(SwapFile* file_swap,LogicalMemory* page_table)
{
  //SWAP jeszze raz napisac
  //zawartosc_ramki_do_swapa przerzucic(assigned_frames_deque.front()
  
  for(uint8_t p=0;p<page_table->getPageTableSize();++p)//pêtla odpowiedzialna za przeniesienie ca³ego programu do swapa
  {
  uint16_t frame=page_table->getFrameNr(p);//podstawia strone ktora zostanie przenisionea

    for(uint16_t i=this->getFrameSize()*frame,j=this->getFrameSize()*file_swap->getFreeFrame();i<this->getFrameSize();++i,++j)//przenosi strone do swapa
    {
    file_swap->getSwap()[j]=this->getStorage()[i];
    }
  
  page_table->setPage(file_swap->getFreeFrame(),0,p);//numer ramki w tablicy stron ustawiony na numer ramki ze swapa valid na 0
  file_swap->setSwapFrame(this->getFrame(frame)->pid,p,file_swap->getFreeFrame());//zapisanie informacji o ramce na swapie w tabeli ramek, 
  this->loggg->logMemory(sop::logger::Logger::Level::INFO,"Page: "+sop::StringConverter::convertToString((int)p)+" is loacted in swap in frame: "+sop::StringConverter::convertToString(file_swap->getFreeFrame())+"and bit valid is: "+ sop::StringConverter::convertToString(page_table->getBitValidInvalid(p)));
  this->setFrame(0,0,frame);//czyscimy ramke przerzucon¹ na swapa
 
  file_swap->popFrontListOfFreeSwapFrames();//usuniecie ramki z listy wolnych ramek w swapie
  file_swap->setNubmerOfFreeSwapFrames(file_swap->getNumberOfFreeSwapFrames()-1);//zmniejsza liczbe wolnych ramek w swapie
  
  }
   //////////////////////zerowanie
  uint16_t reference;
  char zerowanie='0';
  for(int i=0,j=0;i<page_table->getPageTableSize()* this->getFrameSize();++i,++j)
  {
     
    if( j==32)
      j=0;
      

    reference =page_table->getFrameNr(i/this->getFrameSize());//zwraca na której stronie aktualnie operujemy
    reference=reference*this->getFrameSize();//adres pocz¹tkowy komórki w tablicy fizycznej
    this->getStorage()[reference+j]=zerowanie;//wpisanie do pamiêci
  }


  ///////////////////
}

uint8_t sop::memory::PhysicalMemory::getFreeFrames(uint8_t pages_needed,sop::memory::LogicalMemory* table_of_pages,uint16_t pid,SwapFile* file_swap,LogicalMemory* tabela_procesow)
{
 
  while(isEnoughFrames(pages_needed)!=true)
  {

   
    if(file_swap->getIsThereAnyFrameValue(pages_needed)==true)//sprawdzenie czy swap nie jest zapchany
    {

      //LogicalMemory victim_page_table = find(this->frame_table[this->assigned_frames_deque.front()].pid); //funkcja find ma zwrocic tabele stron/proces ktorego ramka jest na 1. miesjscu w kolejce, na podstawie PID
      //majac proces/jego tabele stron szukamy tej strony, ktora ma ta ramke:
      // for(int i=0;i<victim_page_table.getPageTableSize();++i)
      // {
      //   if(victim_page_table->getPage(i)->frame_number == this->assigned_frames_deque.front())
      //     victim_page=i;
      // } 
      
    
    uint8_t tmp_size=tabela_procesow[findBestToSwap(tabela_procesow,pages_needed)].getPageTableSize();//zmienna przetrzymuje
      for(int i=0;i<tabela_procesow[findBestToSwap(tabela_procesow,pages_needed)].getPageTableSize();++i)
        //wrzucenie do listy wolnych ramek te ktore zostana wymiecione
        this->list_of_free_frames.push_back(tabela_procesow[findBestToSwap(tabela_procesow,pages_needed)].getFrameNr(i));
      ///tabela_procesow[findBestToSwap(tabela_procesow,pages_needed)]
       swap(file_swap,&tabela_procesow[findBestToSwap(tabela_procesow,pages_needed)]);//przerzucenie zawartosci struktury ramka do swapa
       this->setNubmerOfFreeFrames(this->getNumberOfFreeFrames()+tmp_size-table_of_pages->getPageTableSize());//zwiêkszenie liczby wolnych ramek
      this->setNumberOfNotFreeFrames(this->getNumberOfFrames() - this->getNumberOfFreeFrames());//zmniejszenie liczby zajêtych ramek


     //ZMIENIC: this->list_of_free_frames.push_back(this->assigned_frames_deque.front());//wrzucenie ramki, ktorej strona zosta³a wymieciona na liste wolnych ramek
     // this->assigned_frames_deque.pop_front();//wyrzucenie ramkie, której strona zosta³a wymieciona z kolejki zaalokowanych ramek

    }
    else 
    {
    
     // this->loggg->logMemory(sop::logger::Logger::Level::SEVERE,"Error");
      return 0;//zabrak³o pamieci
      
    }
  }

  //jeœli poziom wolnych ramek jest odpowiedni po zabraniu ramek to wykonaj:
  for(int i=0;i<pages_needed;++i)
  {

    table_of_pages->setPage(this->list_of_free_frames.front(),0,i);//przydzielenie ramki danej stronie i ustawienie valid na 0 bo jeszcze nie w pamieci
    this->setNubmerOfFreeFrames(this->getNumberOfFreeFrames()-1);//zmniejsza liczbê wolnych ramek
    this->setNumberOfNotFreeFrames(this->getNumberOfFrames() - this->getNumberOfFreeFrames());//zwiêksza liczbê zajêtych ramek
    this->setFrame(pid,i,this->list_of_free_frames.front());//przypisanie strony danej ramce, informacje o tym zapisane w tabeli ramek
    //this->assigned_frames_deque.push_back(this->list_of_free_frames.front());//zapisuje w kolejce która ramka zosta³a przypisana
    this->list_of_free_frames.pop_front();//usuwa przydzielon¹ ramkê z listy wolnych ramek
  }
  return 1;//jest ok

}

//testowe

uint8_t sop::memory::PhysicalMemory::findBestToSwap(LogicalMemory* tabela_procesow,uint8_t pages_needed)
{
  uint8_t victim_process;
  
  uint8_t suma;//ile stron zwolni dany proces
 
  int tmpTab [2][10]={};//tablica odpowiedzialna za zebranie informacji ile dany proces(1 wiersz kolumna) zwolni miejsca(2 wiersz)
                        //zeruje tablice, jesli obie komorki rowne 0 tzn ze nie brac pod uwage

  

  for(int i=0;i<10;++i)
  {
    if( (tabela_procesow[i].getPageTableSize()!=0) &&(tabela_procesow[i].getBitValidInvalid(0)==1) )//sprawdza czy dany proces w ogole jest na podstawie zaalokowanych stron i czy jest w pamiêci g³ównej - pierwsz¹ stronê sprawdza
      if(tabela_procesow[i].getPageTableSize()+this->getNumberOfFreeFrames() >=pages_needed)
       {
        suma=tabela_procesow[i].getPageTableSize()+this->getNumberOfFreeFrames();
        tmpTab[0][i]=i;//zapisze do tablicy pid procesu czyli ze on cos faktycznie zwolni
        tmpTab[1][i]=suma;//zapisze do tablicy ile miejsca zwolni
       }
   }

  //znaleznie pierwszej ofiary
    
  for(int j=0;j<10;++j)
  {
    if(tmpTab[1][j] !=0)
    {
      victim_process=j;
      break;
    }
  }
  //porownanie z innymi
  if(victim_process==9)//sprawdzenie czy ofiara nie jestem ostatnim procesem tzn jest tylko 1 proces
    return victim_process;
  else//jesli nie jest ostatni to sprawdz inne
  {

  for(int j=victim_process+1;j<10;++j){
    if(tmpTab[1][j] !=0)
      if(tmpTab[1][j-1]>tmpTab[1][j])
        victim_process=j;
  }
  return victim_process;   
  }

}


