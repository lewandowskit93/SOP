#ifndef _SOP_PROCESSES_MODULE_H_
#define _SOP_PROCESSES_MODULE_H_

#include <string>
#include ".\sop\system\module.h"
#include <vector>
#include <boost\shared_ptr.hpp>
#include ".\sop\processes\class_process.h"

namespace sop
{
  namespace processes
  {
    /*
      Processes module class.
    */
    class Module : public sop::system::Module
    {
      public:
        explicit Module(sop::system::Kernel *kernel);
        virtual ~Module();
        virtual std::string getClassName() const;
        virtual void initialize();

        //definicja wektora i funkcje go obslugujace
        std::vector <boost::shared_ptr<sop::process::Process>> ProcessVector;        
        void addToVector(boost::shared_ptr<sop::process::Process> objekt);
        void removeFromVector(uint16_t PID);
        void showObjectInList(uint16_t PID);
        boost::shared_ptr<sop::process::Process> findProcess(uint16_t PID);

        //stworzenie glownego procesu numer 0 (shell) z ktorego beda tworzone kolejne procesy.
        //ma przypisane takie wartosci pol ktore spowoduja ze nie bedzie ruszany ani rpzez pamiec ani przez procesor.
        void CreateShellInit();
        //klonowanie dla procesu sztucznegi
        void cloneProcessTrue(boost::shared_ptr<sop::process::Process> Proces1, boost::shared_ptr<sop::process::Process> Proces2);
        //kloniwanie dla procesu prawdziwego
        void cloneProcessFalse(boost::shared_ptr<sop::process::Process> Proces1, boost::shared_ptr<sop::process::Process> Proces2);
        //Stworzenie nowego procesu
        void createNewProcess();
        //stworzenie procesu potomka, ktory ma skopiowane srodowisko z procesu, z ktorego jest stworzony.
        void fork();
        //wyslanie kodu programu do danego procesu co spowoduje wyslanie prosby o alokacje pamieci i przydzial procesora.
        void exec();
      
      protected:

      private:

    };
  }
}

#endif