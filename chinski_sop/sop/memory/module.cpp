#include ".\sop\memory\module.h"
#include ".\sop\system\kernel.h"
#include ".\sop\logger\logger.h"
#include ".\sop\string_converter.h"



sop::memory::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel),physical_drive(4096,32,_kernel->getLogger()),swap_drive(4096,32,_kernel->getLogger())//stworzenie pamiêci fizycznej i pliku wymiany o rozmiarze ca³kowitym, rozmairze 1 ramki
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
  _kernel->getShell()->registerCommand("lsFramesSwap",&sop::memory::Module::cH_showFramesSwap,this);
  _kernel->getShell()->registerCommand("writeToMemory",&sop::memory::Module::cH_writeToMemory,this);
  _kernel->getShell()->registerCommand("readFrame",&sop::memory::Module::cH_readFrame,this);
  _kernel->getShell()->registerCommand("readFrameSwap",&sop::memory::Module::cH_readFrameSwap,this);
  _kernel->getShell()->registerCommand("readByte",&sop::memory::Module::cH_readByte,this);
  _kernel->getShell()->registerCommand("deallocate",&sop::memory::Module::cH_deallocate,this);
}

uint8_t sop::memory::Module::calculatePages(uint16_t program_size)
{
  float number_of_pages=(float)physical_drive.getFrameSize();
  number_of_pages=ceil(((float)program_size/number_of_pages));

  return (uint8_t)number_of_pages;
}

sop::memory::LogicalMemory sop::memory::Module::allocate(uint16_t program_size,uint16_t pid)
{
  if(calculatePages(program_size)>=this->physical_drive.getListForFreeFrames().size() && physical_drive.getDequeFrames().size()==0)
  {
    _kernel->getLogger()->logMemory(sop::logger::Logger::Level::WARNING,"Cannot load program into memory,too big");
    LogicalMemory table_of_pages(0,_kernel->getLogger());
    return table_of_pages;//zwraca pust¹ tablice, gdy program jest za duzy, trzeba dalej spradzic po pageTableSize
  }
  if(this->swap_drive.getIsThereAnyFrameValue()==false)
   {
     LogicalMemory table_of_pages(0,_kernel->getLogger());
     _kernel->getLogger()->logMemory(sop::logger::Logger::Level::SEVERE,"END of memory");
    return table_of_pages;//zwraca pust¹ tablice gdy zabrak³o pamiêci
  }
  
  LogicalMemory table_of_pages(calculatePages(program_size),_kernel->getLogger());//tworzenie tabeli stron dla procesu, o okreœlonej liczbie stron
   uint8_t temp=physical_drive.getFreeFrames(table_of_pages.getPageTableSize(),&table_of_pages,pid,&swap_drive);//wywo³anie funkcji przydzielaj¹cej ramki stronom na podstawie liczby stron w tabeli stron
 if(temp==1)
   _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Page table for process with pid: "+sop::StringConverter::convertToString(pid)+" has beem created");
 else
 {
   _kernel->getLogger()->logMemory(sop::logger::Logger::Level::SEVERE,"END of memory");
   LogicalMemory table_of_pages(0,_kernel->getLogger());
    return table_of_pages;
 }
 
  return table_of_pages;
}

void sop::memory::Module::deallocate(sop::memory::LogicalMemory* page_table)
{
  //wyczyscic pamie fizyzna, nadpisac zeraami//ew swapa i odpowiendio tabela ramek swapa OPCJONALNIE
  uint16_t reference;
  char zerowanie='0';
  for(int i=0,j=0;i<page_table->getPageTableSize()* physical_drive.getFrameSize();++i,++j)
  {
     
    if( j==32)
      j=0;
      

    reference =page_table->getFrameNr(i/physical_drive.getFrameSize());//zwraca na której stronie aktualnie operujemy
    reference=reference*physical_drive.getFrameSize();//adres pocz¹tkowy komórki w tablicy fizycznej
    physical_drive.getStorage()[reference+j]=zerowanie;//wpisanie do pamiêci
  }
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

  char byte;
  if(page_table.getBitValidInvalid(byte_number/physical_drive.getFrameSize())==1)  //czy swap czy physical
  {
  uint8_t page_nr=byte_number/physical_drive.getFrameSize();//obliczenie ktora strona jest podaba przez byte
  uint16_t reference=page_table.getFrameNr(page_nr)*physical_drive.getFrameSize();//ustawienie która komórkê zczytaæ
  byte= physical_drive.getStorage()[reference+byte_number];
  }
  if(page_table.getBitValidInvalid(byte_number/swap_drive.getSwapFrameSize())==0)
  {
  uint8_t page_nr=byte_number/swap_drive.getSwapFrameSize();//obliczenie ktora strona jest podaba przez byte
  uint16_t reference=page_table.getFrameNr(page_nr)*swap_drive.getSwapFrameSize();//ustawienie która komórkê zczytaæ
  byte= swap_drive.getSwap()[reference+byte_number];
  }
  return byte;
}

void sop::memory::Module::write(LogicalMemory page_table, std::string code)
{

  uint16_t reference;
  
  for(int i=0,j=0;i<code.size();++i,++j)
  {
     
    if( j==32)
      j=0;
      

    reference =page_table.getFrameNr(i/physical_drive.getFrameSize());//zwraca na której stronie aktualnie operujemy
    reference=reference*physical_drive.getFrameSize();//adres pocz¹tkowy komórki w tablicy fizycznej
    physical_drive.getStorage()[reference+j]=code.at(i);//wpisanie do pamiêci
  }
  //if(code.empty()==true)
  //error
  //return
  //code size=35
 // uint16_t frame_nr=page_table.getFrameNr(0)* physical_drive.getFrameSize(); //1*32
  //for(int i=frame_nr,j=0;i<code.size();++i,++j)//i=32,i=33 j=1
 // {
  //  physical_drive.getStorage()[i]=code.at(j);//32 at(0),33 at(1)
   // frame_nr=page_table.getFrameNr(j/physical_drive.getFrameSize());//0
 // }
  
      //  adres poczatku=nr ramki +(reference*rozmiarstrony) +i
    //adres strony 

     
    //reference =page_table.getFrameNr(i/physical_drive.getFrameSize());//zwraca na której stronie aktualnie operujemy
    //reference=reference+physical_drive.getFrameSize();//adres pocz¹tkowy komórki w tablicy fizycznej
   // physical_drive.getStorage()[reference+i]=code.at(i);//wpisanie do pamiêci
  
   
  
      
  for(int i=0;i<page_table.getPageTableSize();++i)
  {
    page_table.setBitBalidInvalid(i,1);
  }
  _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Program has been loaded to memory");

 
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
    std::cout<<"Display a frames statistics for physical memory"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  std::list<uint16_t> mylist=physical_drive.getListForFreeFrames();
  std::cout<<"List of free frames on physical drive:"<<std::endl;
  uint16_t counter=1;
  for (std::list<uint16_t>::iterator it=mylist.begin(); it != mylist.end(); ++it)
  {
    std::cout << ' ' << sop::StringConverter::convertToString<uint16_t>(*it);
    if(counter%16==0)
      std::cout<<std::endl;
    counter++;
  }
  std::cout<<std::endl;
  std::cout<<"Number free frames on physical drive: ";
  std::cout << sop::StringConverter::convertToString<uint16_t>(physical_drive.getNumberOfFreeFrames());
  std::cout<<std::endl;
  std::cout<<"Number of taken frames on physical drive: ";
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

void sop::memory::Module::cH_showFramesSwap(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()>1)
  {
    std::cout<<"lsFramesSwap [-h] "<<std::endl;
    std::cout<<"Display a frames statistics for swap file"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  std::list<uint16_t> mylist=swap_drive.getListForFreeSwapFrames();
  std::cout<<"List of free frames on swap:"<<std::endl;
  uint16_t counter=1;
  for (std::list<uint16_t>::iterator it=mylist.begin(); it != mylist.end(); ++it)
  {
    std::cout << ' ' << sop::StringConverter::convertToString<uint16_t>(*it);
    if(counter%16==0)
      std::cout<<std::endl;
    counter++;
  }
  std::cout<<std::endl;
  std::cout<<"Number free frames on swap: ";
  std::cout << sop::StringConverter::convertToString<uint16_t>(swap_drive.getNumberOfFreeSwapFrames());
  std::cout<<std::endl;

  std::cout<<"Is there any space on swap file? ";
  std::cout << sop::StringConverter::convertToString<bool>(swap_drive.getIsThereAnyFrameValue());
  std::cout<<std::endl;
}

void sop::memory::Module::cH_writeToMemory(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=3)
  {
     std::cout<<"writeToMemory [-h] string PID"<<std::endl;
    std::cout<<"Write string of PID to memory"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
    
  }
  LogicalMemory tabelka_stron=allocate(params[1].size(),sop::StringConverter::convertStringTo<uint16_t>(params[2]));
  write(tabelka_stron,params[1]);
   for(int i=0;i<tabelka_stron.getPageTableSize();++i)
  {
    std::cout<<"Page: "+sop::StringConverter::convertToString<uint16_t>(i)+" assigned to frame: "+sop::StringConverter::convertToString<uint16_t>(tabelka_stron.getPage(i)->frame_number) +" bit valid: "+sop::StringConverter::convertToString<uint16_t>(tabelka_stron.getPage(i)->valid_invalid);
    std::cout<<std::endl;
  }
  }

void sop::memory::Module::cH_readFrame(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=2)
  {
    std::cout<<"readFrame [-h] frame_nr"<<std::endl;
    std::cout<<"Read specified frame from physical memory"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  std::string txt="";

  // for(int i=0,j=0;i<code.size();++i,++j)
//  {
 //    
 //   if( j==32)
 //    j=0;
      

   // reference =page_table.getFrameNr(i/physical_drive.getFrameSize());//zwraca na której stronie aktualnie operujemy
 //   reference=reference*physical_drive.getFrameSize();//adres pocz¹tkowy komórki w tablicy fizycznej
    //physical_drive.getStorage()[reference+j]=code.at(i);//wpisanie do pamiêci
 //stare read:
  //int16_t reference =(sop::StringConverter::convertStringTo<uint16_t>(params[1]) * this->physical_drive.getFrameSize());
 // for(int i =reference;i<(reference+ this->physical_drive.getFrameSize());++i)
  //{
 //   txt=txt.append(sop::StringConverter::convertToString(this->physical_drive.getStorage()[reference+i]));
 // }
  
  int16_t reference =(sop::StringConverter::convertStringTo<uint16_t>(params[1]) * this->physical_drive.getFrameSize());
  uint16_t end=reference+ this->physical_drive.getFrameSize();
  for(int i =reference;i<end;++i)
  {
    txt=txt.append(sop::StringConverter::convertToString(this->physical_drive.getStorage()[i]));
  }
  std::cout<<std::endl<<"Frame: "<<sop::StringConverter::convertToString(params[1])<<" contain: "<<std::endl<<txt<<std::endl;
}

void sop::memory::Module::cH_readFrameSwap(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=2)
  {
    std::cout<<"readFrameSwap [-h] frame_nr"<<std::endl;
    std::cout<<"Read specified frame from swap file"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  std::string txt="";
  
  int16_t reference =(sop::StringConverter::convertStringTo<uint16_t>(params[1]) * this->swap_drive.getSwapFrameSize());
  for(int i =reference;i<(reference+ this->swap_drive.getSwapFrameSize());++i)
  {
    txt=txt.append(sop::StringConverter::convertToString(this->swap_drive.getSwap()[reference+i]));
  }
  std::cout<<std::endl<<"Frame: "<<sop::StringConverter::convertToString(params[1])<<" contain: "<<std::endl<<txt<<std::endl;
}

void sop::memory::Module::cH_readByte(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=4)
  {
    std::cout<<"readByte [-h] string PID offset"<<std::endl;
    std::cout<<"Read specified byte, based on program offset, string is program code"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  LogicalMemory tabelka_stron=allocate(params[1].size(),sop::StringConverter::convertStringTo<uint16_t>(params[2]));
  write(tabelka_stron,params[1]);
   for(int i=0;i<tabelka_stron.getPageTableSize();++i)
  {
    std::cout<<"Page: "+sop::StringConverter::convertToString<uint16_t>(i)+" assigned to frame: "+sop::StringConverter::convertToString<uint16_t>(tabelka_stron.getPage(i)->frame_number) +" bit valid: "+sop::StringConverter::convertToString<uint16_t>(tabelka_stron.getPage(i)->valid_invalid);
    std::cout<<std::endl;
  }

   std::cout<<"Byte: "<<sop::StringConverter::convertToString(params[3])<<" is "<< sop::StringConverter::convertToString(read(tabelka_stron,sop::StringConverter::convertStringTo<uint16_t>(params[3])))<<std::endl;
}

void sop::memory::Module::cH_deallocate(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=1)
  {
     std::cout<<"deallocate [-h] "<<std::endl;
    std::cout<<"Delete one of two created pages table(first will be deleted - 2 frames,second 3 frames )"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
    
  }
  std::string kod1="SystemyOperacyjneToJedenZPrzedmiotowKierunkuInformatyka";
  std::string kod2="3,14159 26535 89793 23846 26433 83279 50288 41971 69399 37510 58209 74944 59230 78164 ";

  LogicalMemory tabelka_stron1=allocate(kod1.size(),1);
 
  write(tabelka_stron1,kod1);
   for(int i=0;i<tabelka_stron1.getPageTableSize();++i)
  {
    std::cout<<"Page: "+sop::StringConverter::convertToString<uint16_t>(i)+" assigned to frame: "+sop::StringConverter::convertToString<uint16_t>(tabelka_stron1.getPage(i)->frame_number) +" bit valid: "+sop::StringConverter::convertToString<uint16_t>(tabelka_stron1.getPage(i)->valid_invalid);
    std::cout<<std::endl;
  }
   //////////////////////////////////////
  std::list<uint16_t> mylist=physical_drive.getListForFreeFrames();
  std::cout<<"List of free frames on physical drive:"<<std::endl;
  uint16_t counter=1;
  for (std::list<uint16_t>::iterator it=mylist.begin(); it != mylist.end(); ++it)
  {
    std::cout << ' ' << sop::StringConverter::convertToString<uint16_t>(*it);
    if(counter%16==0)
      std::cout<<std::endl;
    counter++;
  }
  std::cout<<std::endl;
  std::cout<<"Number free frames on physical drive: ";
  std::cout << sop::StringConverter::convertToString<uint16_t>(physical_drive.getNumberOfFreeFrames());
  std::cout<<std::endl;
  std::cout<<"Number of taken frames on physical drive: ";
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
  
   ////////////////////////////////////////////
    LogicalMemory tabelka_stron2=allocate(kod2.size(),2);
   write(tabelka_stron2,kod2);
   for(int i=0;i<tabelka_stron2.getPageTableSize();++i)
  {
    std::cout<<"Page: "+sop::StringConverter::convertToString<uint16_t>(i)+" assigned to frame: "+sop::StringConverter::convertToString<uint16_t>(tabelka_stron2.getPage(i)->frame_number) +" bit valid: "+sop::StringConverter::convertToString<uint16_t>(tabelka_stron2.getPage(i)->valid_invalid);
    std::cout<<std::endl;
  }
   ////////////////////////////////////

  mylist=physical_drive.getListForFreeFrames();
  std::cout<<"List of free frames on physical drive:"<<std::endl;
  counter=1;
  for (std::list<uint16_t>::iterator it=mylist.begin(); it != mylist.end(); ++it)
  {
    std::cout << ' ' << sop::StringConverter::convertToString<uint16_t>(*it);
    if(counter%16==0)
      std::cout<<std::endl;
    counter++;
  }
  std::cout<<std::endl;
  std::cout<<"Number free frames on physical drive: ";
  std::cout << sop::StringConverter::convertToString<uint16_t>(physical_drive.getNumberOfFreeFrames());
  std::cout<<std::endl;
  std::cout<<"Number of taken frames on physical drive: ";
  std::cout << sop::StringConverter::convertToString<uint16_t>(physical_drive.getNumberOfNotFreeFrames());
  std::cout<<std::endl;
  std::cout<<"Deque of taken frames - FIFO: "<<std::endl;
  mydeque=physical_drive.getDequeFrames();
  counter=1;
  for (std::deque<uint16_t>::iterator it=mydeque.begin(); it != mydeque.end(); ++it)
  {
    std::cout << ' ' << sop::StringConverter::convertToString<uint16_t>(*it);
    if(counter%16==0)
      std::cout<<std::endl;
    counter++;
  }
  std::cout<<std::endl;
  ///////////////////
  deallocate(&tabelka_stron1);
  //////
  mylist=physical_drive.getListForFreeFrames();
  std::cout<<"List of free frames on physical drive:"<<std::endl;
  counter=1;
  for (std::list<uint16_t>::iterator it=mylist.begin(); it != mylist.end(); ++it)
  {
    std::cout << ' ' << sop::StringConverter::convertToString<uint16_t>(*it);
    if(counter%16==0)
      std::cout<<std::endl;
    counter++;
  }
  std::cout<<std::endl;
  std::cout<<"Number free frames on physical drive: ";
  std::cout << sop::StringConverter::convertToString<uint16_t>(physical_drive.getNumberOfFreeFrames());
  std::cout<<std::endl;
  std::cout<<"Number of taken frames on physical drive: ";
  std::cout << sop::StringConverter::convertToString<uint16_t>(physical_drive.getNumberOfNotFreeFrames());
  std::cout<<std::endl;
  std::cout<<"Deque of taken frames - FIFO: "<<std::endl;
  mydeque=physical_drive.getDequeFrames();
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

