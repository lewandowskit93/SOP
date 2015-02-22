#ifndef _SOP_MEMORY_MODULE_H_
#define _SOP_MEMORY_MODULE_H_

#include <string>
#include <math.h>
#include <iostream>
#include <deque>
#include <vector>
#include ".\sop\system\module.h"
#include ".\sop\memory\logical_memory.h"
#include ".\sop\memory\physical_memory.h"
#include ".\sop\memory\swap.h"



namespace sop
{
  namespace memory
  {
    /*
    Memory module class.
    */

         
    class Module : public sop::system::Module
    {
    public:
      explicit Module(sop::system::Kernel *kernel);
      virtual ~Module();
      virtual std::string getClassName() const;
      virtual void initialize();

      LogicalMemory allocate(uint16_t program_size,uint16_t pid);//przydzia³ ramek stronom, zwraca wskaznik na tabelê stron
      void deallocate(LogicalMemory* page_table);//zwalnianie pamiêci
      char read(LogicalMemory page_table, uint16_t byte_number);//zczytanie bajtu z pamiêci
      void write(LogicalMemory page_table, std::string code);//wpisanie do pamieci ca³ego kodu w postaci stringa
      ///////////testowe rzeczy
          //void addToListaProcesow(LogicalMemory page_table, uint16_t pid);//dodaje do listy procesow nowy proces
          //void removeFromListaProcesow(uint16_t pid);//usuwa proces o wskazanym PID z listy procesow
          //LogicalMemory findByPidListaProcesow(uint16_t pid);//zwraca tabele stron procesu na podstawie PID
          //proces findProces(uint16_t pid);//zwraca strukture proces na podstawie pid z listy procesow
          //LogicalMemory findProcesToSwap(uint16_t size_needed);//odpowiada za znalezienie procesu ktory trzeba przerzucic na swapa
      LogicalMemory* tablica_procesow;//tablica procesów, lokalizacja=pid, 10 procesow mozliwych=procesy pid 0-9
      std::deque<int> wolne_pid;
      bool check_pid(uint16_t pid);//zwraca czy pid jest wolne,
      void usun_pid(uint16_t pid);//usunie pid z kolejki
      void dodaj_pid(uint16_t pid);//doda pid do kolejki
      ///////////////////////
    protected:

    private:
      PhysicalMemory physical_drive;//nasza pamiêæ fizyczna, pierwszy argument to rozmair w bajtach, a drugi rozmiar ramki
      uint8_t calculatePages(uint16_t program_size); //zwraca liczbê stron niezbêdn¹ dla danego programu
      SwapFile swap_drive;
      //testowe sprawy
          // std::list<proces> lista_procesow;
      void sop::memory::Module::cH_showPID(const std::vector<const std::string> & params);//pokazuje jakie wolne sa PIDY
      void sop::memory::Module::cH_showProcesses(const std::vector<const std::string> & params);//pokazuje jakie proces ma ile stron
      void sop::memory::Module::cH_showPagesTable(const std::vector<const std::string> & params);//pokazuje tablice stron danego procesu na podstawie PID
      void sop::memory::Module::cH_writeByte(const std::vector<const std::string> & params);//zapisuje wskazaby bajt do ramki, na podstawie PID i adresu logicznego
      //komendy
      void cH_allocate(const std::vector<const std::string> & params); //definicja polecenia do shella odpowidzialnej za przydzielenie pamiêci
      void cH_showFrames(const std::vector<const std::string> & params);//pokazuje statystki dla tabeli ramek pamiêci fizycznej
      void cH_showFramesSwap(const std::vector<const std::string> & params);//pokazuje statystki dla tabeli ramek swapa
      void cH_writeToMemory(const std::vector<const std::string> & params);//zapisuje podany string i odpowiednio PID do pamiêci fizycznej
      void cH_readFrame(const std::vector<const std::string> & params);//odczytuje wskazan¹ ramkê z pamiêci fizycznej
      void cH_readFrameSwap(const std::vector<const std::string> & params);//odczytuje wskazan¹ ramkê ze swapa
      void cH_readByte(const std::vector<const std::string> & params);//odczytuje wskzany bajt ze swapa/pamiêci fizycznej
      void cH_deallocate(const std::vector<const std::string> & params);//stworzy 2 tablice stron i usunie pierwszy z nich
    };
  }
}

#endif


