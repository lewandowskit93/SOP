#include ".\sop\memory\module.h"
#include ".\sop\system\kernel.h"
#include ".\sop\logger\logger.h"
#include ".\sop\string_converter.h"



sop::memory::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel),physical_drive(256,16,_kernel->getLogger()),swap_drive(256,16,_kernel->getLogger())//stworzenie pamiêci fizycznej i pliku wymiany o rozmiarze ca³kowitym, rozmairze 1 ramki
{

}

sop::memory::Module::~Module()
{
  delete[] tablica_procesow;
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
  

  ///testowe
  _kernel->getShell()->registerCommand("showPID",&sop::memory::Module::cH_showPID,this);
  _kernel->getShell()->registerCommand("showProcesses",&sop::memory::Module::cH_showProcesses,this);
  _kernel->getShell()->registerCommand("showPagesTable",&sop::memory::Module::cH_showPagesTable,this);
  _kernel->getShell()->registerCommand("writeByte",&sop::memory::Module::cH_writeByte,this);

 
  tablica_procesow = new LogicalMemory [10];
  //std::deque<int>::iterator it = wolne_pid.begin(); 
  for (int i=0;i<10;++i)//wrzuecenie do kolejki ktory pid sa wolne
  {
    wolne_pid.push_back(i);
  }
  //for (unsigned i=0; i<wolne_pid.size(); i++)
  //  std::cout << ' ' << wolne_pid.at(i);
  
}

uint8_t sop::memory::Module::calculatePages(uint16_t program_size)
{
  float number_of_pages=(float)physical_drive.getFrameSize();
  number_of_pages=ceil(((float)program_size/number_of_pages));

  return (uint8_t)number_of_pages;
}
//TODO przetestwoaæ dobrze moment zapchania pamiêci i gdy program za duzy
sop::memory::LogicalMemory sop::memory::Module::allocate(uint16_t program_size,uint16_t pid)
{
  if(calculatePages(program_size)>this->physical_drive.getListForFreeFrames().size() && this->swap_drive.getIsThereAnyFrameValue(0)==false)
  {
    _kernel->getLogger()->logMemory(sop::logger::Logger::Level::WARNING,"Cannot load program into memory,too big");
    LogicalMemory table_of_pages(0,_kernel->getLogger());
    return table_of_pages;//zwraca pust¹ tablice, gdy program jest za duzy, trzeba dalej spradzic po pageTableSize
  }
  if(this->swap_drive.getIsThereAnyFrameValue(0)==false)
   {
     LogicalMemory table_of_pages(0,_kernel->getLogger());
     _kernel->getLogger()->logMemory(sop::logger::Logger::Level::SEVERE,"END of memory");
    return table_of_pages;//zwraca pust¹ tablice gdy zabrak³o pamiêci
  }
  
  LogicalMemory table_of_pages(calculatePages(program_size),_kernel->getLogger());//tworzenie tabeli stron dla procesu, o okreœlonej liczbie stron
  uint8_t temp=physical_drive.getFreeFrames(table_of_pages.getPageTableSize(),&table_of_pages,pid,&swap_drive,tablica_procesow);//wywo³anie funkcji przydzielaj¹cej ramki stronom na podstawie liczby stron w tabeli stron
 if(temp==1)
 {
   _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Page table for process with pid: "+sop::StringConverter::convertToString(pid)+" has beem created");
    usun_pid(pid);
 }
 else
 {
   _kernel->getLogger()->logMemory(sop::logger::Logger::Level::SEVERE,"END of memory");
   LogicalMemory table_of_pages(0,_kernel->getLogger());
    return table_of_pages;
 }
 for(int i=0;i<table_of_pages.getPageTableSize();++i)
  {
    table_of_pages.setBitBalidInvalid(i,1);
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
      //this->physical_drive.FindAndEraseFromDeque(page_table->getFrameNr(i));//znalezienie i usuniecie ramki z kolejki zajetych ramek
    }
    if(page_table->getBitValidInvalid(i)==0)
    {
      this->swap_drive.setSwapFrame(0,0,i);//zmiana w tabeli ramek swapa
      this->swap_drive.pushEndListOfFreeSwapFrames(page_table->getFrameNr(i));//wrzucenie danej ramki do listy wolnych ramek
      this->swap_drive.setNubmerOfFreeSwapFrames(this->swap_drive.getNumberOfFreeSwapFrames()+1);//zwiekszenie liczby wolnych ramek

    }
  }




}
//TODO przeniesc ze swapa gdy chcemy odczytac proces juz wczytany
char sop::memory::Module::read(LogicalMemory page_table, uint16_t byte_number)
{

  char byte;
  if(page_table.getBitValidInvalid(byte_number/physical_drive.getFrameSize())==1)  //czy swap czy physical
  {
  uint8_t page_nr=byte_number/physical_drive.getFrameSize();//obliczenie ktora strona jest podaba przez byte
  uint16_t reference=page_table.getFrameNr(page_nr)*physical_drive.getFrameSize();//ustawienie która komórkê zczytaæ
  byte_number=(int)byte_number%physical_drive.getFrameSize();
  byte= physical_drive.getStorage()[reference+byte_number];
  }
  if(page_table.getBitValidInvalid(byte_number/swap_drive.getSwapFrameSize())==0)
  {
  uint8_t page_nr=byte_number/swap_drive.getSwapFrameSize();//obliczenie ktora strona jest podaba przez byte
  uint16_t reference=page_table.getFrameNr(page_nr)*swap_drive.getSwapFrameSize();//ustawienie która komórkê zczytaæ
   byte_number=(int)byte_number%physical_drive.getFrameSize();
  byte= swap_drive.getSwap()[reference+byte_number];
  }
  return byte;
}

void sop::memory::Module::write(LogicalMemory page_table, std::string code)
{

  uint16_t reference;
  
  for(int i=0,j=0;i<code.size();++i,++j)
  {
     
    if( j==physical_drive.getFrameSize())////////////////////////////////////////////////////////
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
  
   
  
      
  
  _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Program has been loaded to memory");

 
}

//testowe

/*
sop::memory::proces::proces(sop::logger::Logger* log):
  page_table(0,log)
{

}

bool sop::memory::proces::operator==(const proces & st)
{
    return pid == st.pid;
             
}

void sop::memory::Module::addToListaProcesow(LogicalMemory page_table, uint16_t pid)
{
  proces tmp(_kernel->getLogger());//tymczasowa struktura
  tmp.pid=pid;//przypisanie pida
  tmp.page_table=page_table;//przypisanie tabeli stron
  this->lista_procesow.push_front(tmp);//przerzucenie struktury do listy
}

sop::memory::proces sop::memory::Module::findProces(uint16_t pid)
{
  proces tmp(_kernel->getLogger());
  
  for (std::list<proces>::iterator it=this->lista_procesow.begin(); it!=this->lista_procesow.end(); ++it)//petla szukajaca struktury o podanym pid
  {
    if(it->pid==pid)
    {
      tmp.page_table=it->page_table;
      tmp.pid=it->pid;
      
      
    }
  }
  return tmp;
  //TO DO zabezpieczenie przed szukaniem procesu ktorego nie ma
}

void sop::memory::Module::removeFromListaProcesow(uint16_t pid)
{
  uint16_t temp=0;//zmienna sluzaca do przeusniecia iteratora tak zeby usunac to co trzeba
  std::list<proces>::iterator it = this->lista_procesow.begin();//iterator na poczatek listy
  for (uint16_t i=0; i<this->lista_procesow.size(); ++i)//znalezienie ramki w liscie i ustawienie tempa
  {
    if(it->pid==pid)
      break;
    temp=temp+1;


  }
  this->lista_procesow.erase (it+temp,it+temp+1);//usuniecie znalezionego elementu z kolejki
}

sop::memory::LogicalMemory sop::memory::Module::findByPidListaProcesow(uint16_t pid)
{
  proces tmp(_kernel->getLogger());
  for (std::list<proces>::iterator it=this->lista_procesow.begin(); it!=this->lista_procesow.end(); ++it)//petla szukajaca struktury o podanym pid
  {
    if(it->pid==pid)
      tmp=*it;
  }
  return tmp.page_table;
}
*/

bool sop::memory::Module::check_pid(uint16_t pid)
{
  bool finded=false;
  
 
  for (uint16_t i=0; i<this->wolne_pid.size(); ++i)//znalezienie ramki w liscie i ustawienie tempa
  {
    if(wolne_pid.at(i)==pid)
    {
      finded=true;
      break;
    }
    
  }
 
  return finded;
    
  
}

void sop::memory::Module::usun_pid(uint16_t pid)
{
   uint16_t temp=0;//zmienna sluzaca do przeusniecia iteratora tak zeby usunac to co trzeba
  std::deque<int>::iterator it = this->wolne_pid.begin();//iterator na poczatek listy
  for (uint16_t i=0; i<this->wolne_pid.size(); ++i)//znalezienie ramki w liscie i ustawienie tempa
  {
    if(wolne_pid.at(i)==pid)
     break;
    
    temp=temp+1;
    
  }
    this->wolne_pid.erase (it+temp,it+temp+1);//usuniecie znalezionego elementu z kolejki
}

void sop::memory::Module::dodaj_pid(uint16_t pid)
{
  wolne_pid.push_back(pid);

}

void sop::memory::Module::cH_showPID(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()>1)
  {
     std::cout<<"showPID [-h] "<<std::endl;
    std::cout<<"Display free PID"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
    
  }
  std::cout<<"Free PID :"<<std::endl;
  for (unsigned i=0; i<wolne_pid.size(); i++)
    std::cout << ' ' << wolne_pid.at(i);

  std::cout<<std::endl;
  }

void sop::memory::Module::cH_showProcesses(const std::vector<const std::string> & params)
{
  
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()>1)
  {
     std::cout<<"showProcesses [-h] "<<std::endl;
    std::cout<<"Display processes PID ans size of page table"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
    
  }
  
  for (unsigned i=0; i<10; ++i)
    std::cout<<"Process with PID: "<<i<<" have: "<<(uint16_t)tablica_procesow[i].getPageTableSize()<<" pages"<<std::endl;

  
}

void sop::memory::Module::cH_showPagesTable(const std::vector<const std::string> & params)
{
  
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=2)
  {
     std::cout<<"showPagesTable [-h] PID"<<std::endl;
    std::cout<<"Display pages table  specified proccess by PID"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
    
  }
  uint16_t temp_pid=sop::StringConverter::convertStringTo<uint16_t>(params[1]);
  std::cout<<"Page\tFrame\tValid\\Invalid"<<std::endl;

  for(uint8_t i=0;i<tablica_procesow[temp_pid].getPageTableSize();++i)
  {
    std::cout<<(uint16_t)i<<"\t"<<(uint16_t)tablica_procesow[temp_pid].getFrameNr(i)<<"\t"<<(uint16_t)tablica_procesow[temp_pid].getBitValidInvalid(i)<<std::endl;
  }
 

  
}

void sop::memory::Module::cH_writeByte(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=4)
  {
     std::cout<<"writeByte [-h] string PID logical_address"<<std::endl;
    std::cout<<"Write byte to memory, specified by process PID and logical address"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
    
  }

  int page=(int)floor(( (float)(sop::StringConverter::convertStringTo<uint16_t>(params[3])) / physical_drive.getFrameSize() ));//obliczenie ktora strona to bedzie
  
  int offset=(int)(sop::StringConverter::convertStringTo<uint16_t>(params[3]))%physical_drive.getFrameSize();//obliczenie offsetu

  std::string txt =params[1];
  txt=txt.substr(0,1);//przycina stringa do jednej wartosci,w sumie niepotrzebne,ale skoro i tak wpsiuje 0 wartoœæ

  uint16_t temp_pid=sop::StringConverter::convertStringTo<uint16_t>(params[2]);

  uint16_t reference;//odpowiada za miejsce w ktore nalezy wpisac bajt

  reference=tablica_procesow[temp_pid].getFrameNr(page)*physical_drive.getFrameSize();
  reference=reference+offset;
 
  physical_drive.getStorage()[reference]=txt.at(0);//wpisanie do pamiêci

  
  _kernel->getLogger()->logMemory(sop::logger::Logger::Level::INFO,"Byte has been loaded to memory");

}
  
//kody komend

void sop::memory::Module::cH_allocate(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=3)
  {
    std::cout<<"allocate [-h] Program_size PID"<<std::endl;
    std::cout<<"Allocate memory with specified program size and PID(from 0 to 9)"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  if(check_pid(sop::StringConverter::convertStringTo<uint16_t>(params[2]))==false)//sprawdza czy prawid³owy jest pid
  {
    std::cout<<"Wrong PID"<<std::endl;
  }
  else
  {
    uint16_t temp_pid=sop::StringConverter::convertStringTo<uint16_t>(params[2]);
    
  tablica_procesow[temp_pid]=allocate(sop::StringConverter::convertStringTo<uint16_t>(params[1]),sop::StringConverter::convertStringTo<uint16_t>(params[2]));
  
  
  std::cout<<"This number of pages has been created: "<<sop::StringConverter::convertToString<uint16_t>( tablica_procesow[temp_pid].getPageTableSize())<<std::endl;
  for(int i=0;i< tablica_procesow[temp_pid].getPageTableSize();++i)
  {
    std::cout<<"Page: "+sop::StringConverter::convertToString<uint16_t>(i)+" assigned to frame: "+sop::StringConverter::convertToString<uint16_t>( tablica_procesow[temp_pid].getPage(i)->frame_number) +" bit valid: "+sop::StringConverter::convertToString<uint16_t>( tablica_procesow[temp_pid].getPage(i)->valid_invalid);
    std::cout<<std::endl;
   
  }
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
  //usuniete: stronicowanie(ca³y program przenoszony) nie wymaga kolejki
  /*
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
 */
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
  std::cout << sop::StringConverter::convertToString<bool>(swap_drive.getIsThereAnyFrameValue(0));//0 bo ¿adnej nie potrzebujemy
  std::cout<<std::endl;
}

void sop::memory::Module::cH_writeToMemory(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=3)
  {
     std::cout<<"writeToMemory [-h] string PID"<<std::endl;
    std::cout<<"Write string of PID to memory"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
    
  }//wyraz ze spacjami trzeba zapisac w " "
  //cudzys³ów z \"
 
  write(tablica_procesow[sop::StringConverter::convertStringTo<uint16_t>(params[2])],params[1]);
   
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
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=3)
  {
    std::cout<<"readByte [-h]  PID offset"<<std::endl;
    std::cout<<"Read specified byte, based on program offset"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
  }
  
   

  std::cout<<"Byte: "<<sop::StringConverter::convertToString(params[2])<<" is "<< sop::StringConverter::convertToString(read(tablica_procesow[sop::StringConverter::convertStringTo<uint16_t>(params[1])],sop::StringConverter::convertStringTo<uint16_t>(params[2])))<<std::endl;
}

void sop::memory::Module::cH_deallocate(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()!=2)
  {
     std::cout<<"deallocate [-h] PID"<<std::endl;
    std::cout<<"Delete process with specified PID"<<std::endl;
    return; // wyjdzie z tej funkcji, ¿eby nie trzeba by³o robiæ else
    
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
  //usuniete: stronicowanie(ca³y program przenoszony) nie wymaga kolejki
  /*
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
  */
   ////////////////////////////////////////////
   
  ///////////////////
  deallocate(&tablica_procesow[sop::StringConverter::convertStringTo<uint16_t>(params[1])]);
  ///testowe
  dodaj_pid(sop::StringConverter::convertStringTo<uint16_t>(params[1]));
  LogicalMemory* tmp=new LogicalMemory;
  tablica_procesow[sop::StringConverter::convertStringTo<uint16_t>(params[1])]=*tmp;

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
  //usuniete: stronicowanie(ca³y program przenoszony) nie wymaga kolejki
  /*
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
  */
}

