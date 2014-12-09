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

        //definicje komend shellowskich
        void cH_showprocess(const std::vector<const std::string> & params);
        void cH_kill(const std::vector<const std::string> & params);
        void cH_fork(const std::vector<const std::string> & params);
        void cH_exec(const std::vector<const std::string> & params);




        //definicja wektora
        std::vector <boost::shared_ptr<sop::process::Process>> ProcessVector;        
        //dodanie procesu do wektora
        void addToVector(boost::shared_ptr<sop::process::Process> objekt);
        //wyszukanie procesu po PID'dzie i usuwa go z tablicy
        void removeFromVector(uint16_t PID);
        //wyszukanie procesu po PID'dzie
        boost::shared_ptr<sop::process::Process> findProcess(uint16_t PID);
        //stworzenie glownego procesu numer 0 (shell) z ktorego beda tworzone kolejne procesy.
        //ma przypisane takie wartosci pol ktore spowoduja ze nie bedzie ruszany ani rpzez pamiec ani przez procesor.
        void CreateShellInit();
        //Stworzenie nowego procesu
        boost::shared_ptr<sop::process::Process> createNewProcess();
        //stworzenie procesu potomka, ktory ma skopiowane srodowisko z procesu, z ktorego jest stworzony.
        void fork(boost::shared_ptr<sop::process::Process>);
        //
        void exec();
        //pobierz nowy PID
        uint16_t getNewPID();
        //proces macierzysty czeka az wykona sie potomek
        void wait();
        //proces zostaje zabity
        void kill();
      
      protected:

      private:

    };
  }
}

#endif