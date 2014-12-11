#ifndef _SOP_MEMORY_MODULE_H_
#define _SOP_MEMORY_MODULE_H_

#include <string>
#include <math.h>
#include <iostream>
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

      LogicalMemory allocate(uint16_t program_size,uint16_t pid);//przydzia� ramek stronom, zwraca wskaznik na tabel� stron
      void deallocate(LogicalMemory* page_table);//zwalnianie pami�ci
      char read(LogicalMemory page_table, uint16_t byte_number);//zczytanie bajtu z pami�ci
      void write(LogicalMemory page_table, std::string code);//wpisanie do pamieci ca�ego kodu w postaci stringa

    protected:

    private:
      PhysicalMemory physical_drive;//nasza pami�� fizyczna, pierwszy argument to rozmair w bajtach, a drugi rozmiar ramki
      uint8_t calculatePages(uint16_t program_size); //zwraca liczb� stron niezb�dn� dla danego programu
      SwapFile swap_drive;
      //komendy
      void cH_allocate(const std::vector<const std::string> & params); //definicja polecenia do shella odpowidzialnej za przydzielenie pami�ci
      void cH_showFrames(const std::vector<const std::string> & params);
      void cH_showFramesSwap(const std::vector<const std::string> & params);
      void cH_writeToMemory(const std::vector<const std::string> & params);
      void cH_readFrame(const std::vector<const std::string> & params);
    };
  }
}

#endif


