#include ".\sop\memory\module.h"
#include ".\sop\system\kernel.h"
#include ".\sop\logger\logger.h"
#include ".\sop\string_converter.h"



sop::memory::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel),physical_drive(1024,32,_kernel->getLogger()),swap_drive(1024,32,_kernel->getLogger())//stworzenie pamiêci fizycznej i pliku wymiany o rozmiarze ca³kowitym, rozmairze 1 ramki
{

}

sop::memory::Module::~Module()
{

}

std::string sop::memory::Module::getClassName() const
{
  return "sop::memory::Module";
}

void sop::memory::Module::initialize()
{
  _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Memory has been created");
  _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Frame size:"+sop::StringConverter::convertToString<uint16_t>(physical_drive.getFrameSize()));
  _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Physical memory size:"+sop::StringConverter::convertToString<uint16_t>(physical_drive.getStorageSize()));
  _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Swap file size:"+sop::StringConverter::convertToString<uint16_t>(swap_drive.getSwapSize()));
  //inicjalizacja komend
  
  _kernel->getShell()->registerCommand("allocate",&sop::memory::Module::cH_allocate,this);
  _kernel->getShell()->registerCommand("lsFrames",&sop::memory::Module::cH_showFrames,this);
 
}

uint8_t sop::memory::Module::calculatePages(uint16_t program_size)
{
  float number_of_pages=(float)physical_drive.getFrameSize();
  number_of_pages=ceil(((float)program_size/number_of_pages));
  
  return (uint8_t)number_of_pages;
}

sop::memory::LogicalMemory sop::memory::Module::allocate(uint16_t program_size,uint16_t pid)
{
  LogicalMemory table_of_pages(calculatePages(program_size),_kernel->getLogger());//tworzenie tabeli stron dla procesu, o okreœlonej liczbie stron
  physical_drive.getFreeFrames(table_of_pages.getPageTableSize(),&table_of_pages,pid,&swap_drive);//wywo³anie funkcji przydzielaj¹cej ramki stronom na podstawie liczby stron w tabeli stron
  _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Page table for process with pid: "+sop::StringConverter::convertToString(pid)+" has beem created");
   return table_of_pages;
}

void sop::memory::Module::deallocate(sop::memory::LogicalMemory* page_table)
{
   //wyczyscic pamie fizyzna, nadpisac zeraami//ew swapa i odpowiendio tabela ramek swapa OPCJONALNIE
  for(int i=0;i<page_table->getPageTableSize();++i)//odpowiada za zmiane danych w tabeli ramek pamiêci fizycznje/swapa na podstawie ka¿dej ze stron
  {
    if(page_table->getBitValidInvalid(i)==1)
    {
      this->physical_drive.setFrame(0,0,page_table->getFrameNr(i));//zmiana w tabeli ramek w pamiêci fizycznej
      this->physical_drive.pushEndListOfFreeFrames(page_table->getFrameNr(i));//wrzucenie danej ramki do listy wolnych ramek
      this->physical_drive.setNubmerOfFreeFrames(this->physical_drive.getNumberOfFreeFrames()+1);//zwiekszenie liczby wolnych ramek
      this->physical_drive.setNumberOfNotFreeFrames(this->physical_drive.getNumberOfFrames()-this->physical_drive.getNumberOfFreeFrames());//zmniejszenie liczby zajetych ramek
      this->physical_drive.FindAndEraseFromDeque(page_table->getFrameNr(i));//znalezienie i usuniecie ramki z kolejki zajetych ramek
    }
    if(page_table->getBitValidInvalid(i)==0)
    {
      this->swap_drive.setSwapFrame(0,0,i);//zmiana w tabeli ramek swapa
      this->swap_drive.pushEndListOfFreeSwapFrames(page_table->getFrameNr(i));//wrzucenie danej ramki do listy wolnych ramek
      this->swap_drive.setNubmerOfFreeSwapFrames(this->swap_drive.getNumberOfFreeSwapFrames()+1);//zwiekszenie liczby wolnych ramek
      
    }
  }
  

  

}

char sop::memory::Module::read(LogicalMemory page_table, uint16_t byte_number)
{
  uint8_t page_nr=byte_number/physical_drive.getFrameSize();//obliczenie ktora strona jest podaba przez byte
  uint16_t reference=page_table.getFrameNr(byte_number)*physical_drive.getFrameSize();//ustawienie która komórkê zczytaæ
  char byte= physical_drive.getStorage()[reference];

  return byte;
}

void sop::memory::Module::write(LogicalMemory page_table, std::string code)
{
  //if(code.empty()==true)
    //error
  //return
  for(uint16_t i=0;i<code.length();++i)
  {
    for(uint8_t j=0;j<physical_drive.getFrameSize();++j)
    {
      physical_drive.getStorage()[page_table.getFrameNr(i/physical_drive.getFrameSize())*physical_drive.getFrameSize()+j]=code.at(i);
    }
  }
  for(int i=0;i<page_table.getPageTableSize();++i)
  {
    page_table.setBitBalidInvalid(i,1);
  }
   _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Program has been loaded to memory");

   //bit valid na 1
}

//kody komend

void sop::memory::Module::cH_allocate(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=3)
  {
    std::cout<<"allocate [-h] Program_size PID"<<std::endl;
    std::cout<<"Allocate memory with specified program size and PID"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  LogicalMemory tabelka_stron=allocate(sop::StringConverter::convertStringTo<uint16_t>(params[1]),sop::StringConverter::convertStringTo<uint16_t>(params[2]));
 // std::cout<<"program size:"<<sop::StringConverter::convertToString(params[1]);
  
  std::cout<<"This number of pages has been created: "<<sop::StringConverter::convertToString<uint16_t>(tabelka_stron.getPageTableSize())<<std::endl;
  for(int i=0;i<tabelka_stron.getPageTableSize();++i)
  {
    std::cout<<"Page: "+sop::StringConverter::convertToString<uint16_t>(i)+" assigned to frame: "+sop::StringConverter::convertToString<uint16_t>(tabelka_stron.getPage(i)->frame_number) +" bit valid: "+sop::StringConverter::convertToString<uint16_t>(tabelka_stron.getPage(i)->valid_invalid);
    std::cout<<std::endl;
  }
}

void sop::memory::Module::cH_showFrames(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()>1)
  {
    std::cout<<"lsFrames [-h] "<<std::endl;
    std::cout<<"Display a frames statistics"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  std::list<uint16_t> mylist=physical_drive.getListForFreeFrames();
  std::cout<<"List of free frames:"<<std::endl;
  uint16_t counter=1;
  for (std::list<uint16_t>::iterator it=mylist.begin(); it != mylist.end(); ++it)
  {
    std::cout << ' ' << sop::StringConverter::convertToString<uint16_t>(*it);
    if(counter%16==0)
      std::cout<<std::endl;
    counter++;
  }
  std::cout<<std::endl;
  std::cout<<"Number free frames: ";
  std::cout << sop::StringConverter::convertToString<uint16_t>(physical_drive.getNumberOfFreeFrames());
   std::cout<<std::endl;
  std::cout<<"Number of taken frames: ";
  std::cout << sop::StringConverter::convertToString<uint16_t>(physical_drive.getNumberOfNotFreeFrames());
  std::cout<<std::endl;
  std::cout<<"Deque of taken frames - FIFO: "<<std::endl;
  std::deque<uint16_t> mydeque=physical_drive.getDequeFrames();
  counter=1;
   for (std::deque<uint16_t>::iterator it=mydeque.begin(); it != mydeque.end(); ++it)
  {
    std::cout << ' ' << sop::StringConverter::convertToString<uint16_t>(*it);
    if(counter%16==0)
      std::cout<<std::endl;
    counter++;
  }
    std::cout<<std::endl;
}



