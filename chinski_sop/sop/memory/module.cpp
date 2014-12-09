#include ".\sop\memory\module.h"
#include <math.h>

sop::memory::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel),physical_drive(4096,32),swap_drive(4096,32)//stworzenie pamiêci fizycznej i pliku wymiany
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
  
}

uint8_t sop::memory::Module::calculatePages(uint16_t program_size)
{
  float number_of_pages=(float)physical_drive.getFrameSize();
  number_of_pages=ceil(number_of_pages/program_size);
  
  return (uint8_t)number_of_pages;
}

sop::memory::LogicalMemory sop::memory::Module::allocate(uint16_t program_size,uint16_t pid)
{
  LogicalMemory table_of_pages(calculatePages(program_size));//tworzenie tabeli stron dla procesu, o okreœlonej liczbie stron
  physical_drive.getFreeFrames(table_of_pages.getPageTableSize(),&table_of_pages,pid,&swap_drive);//wywo³anie funkcji przydzielaj¹cej ramki stronom na podstawie liczby stron w tabeli stron
  //?dobrze przekazuje table of pages do getFreeFrames??
  return table_of_pages;
}

void sop::memory::Module::deallocate(sop::memory::LogicalMemory* page_table)
{
   //wyczyscic pamie fizyzna, nadpisac zeraami//ew swapa i odpowiendio tabela ramek swapa OPCJONALNIE
  for(int i=0;i<page_table->getPageTableSize();++i)//odpowiada za zmiane danych w tabeli ramek pamiêci fizycznje/swapa na podstawie ka¿dej ze stron
  {
    if(page_table->getBitValidInvalid(i)==1)
    {
      this->physical_drive.setFrame(0,0,i);//zmiana w tabeli ramek w pamiêci fizycznej
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

/*
void::sop::memory::Module::write(char data_block[32] ,LogicalMemory page_table,uint8_t page_nr)
 {
int j=0;
for(int i=page_table.getFrameNr(page_nr)*this->physical_drive.getFrameSize();i<this->physical_drive.getFrameSize();++i)
{
 this->physical_drive.getStorage()[i]=data_block[j];
 j=j+1;
}

}
*/
char sop::memory::Module::read(LogicalMemory page_table, uint16_t byte_number)
{
  uint8_t page_nr=byte_number/physical_drive.getFrameSize();//obliczenie ktora strona jest podaba przez byte
  uint16_t reference=page_table.getFrameNr(byte_number)*physical_drive.getFrameSize();//ustawienie która komórkê zczytaæ
  char byte= physical_drive.getStorage()[reference];
  return byte;
}


