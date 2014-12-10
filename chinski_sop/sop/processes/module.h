#ifndef _SOP_PROCESSES_MODULE_H_
#define _SOP_PROCESSES_MODULE_H_

#include <string>
#include ".\sop\system\module.h"
#include <vector>
#include <queue>
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
        //polecenie shelllowskie wypisujace na ekran informacje o procesie
        void cH_showprocess(const std::vector<const std::string> & params);
        //polecenie shellowskie zabijajace proces o podanym PID
        void cH_kill(const std::vector<const std::string> & params);
        //polecenie shellowskie tworzocy preoces potomka
        void cH_fork(const std::vector<const std::string> & params);
        //polecenie wywolujace funkcje exec
        void cH_exec(const std::vector<const std::string> & params);
        //definicja wektora
        std::vector <boost::shared_ptr<sop::process::Process>> ProcessVector;        
        //funkcja wstawiajaca proces do wektora procesow
        void addToVector(boost::shared_ptr<sop::process::Process> object);
        //funkcja usuwajaca proces z wektora procesow
        void removeFromVector(uint16_t PID);
        //funkcja wyszukujaca proces o podanym PID
        boost::shared_ptr<sop::process::Process> findProcess(uint16_t PID);
        //funkcja tworzaca proces init/shell
        void CreateShellInit();
        //funkcja tworzaca nowy proces
        boost::shared_ptr<sop::process::Process> createNewProcess();
        //funkcja tworzaca nowego potomka
        void fork(boost::shared_ptr<sop::process::Process>);
        //funkcja wpisujaca kod programu do pamieci Fiszy
        void exec(std::string, boost::shared_ptr<sop::process::Process>);
        //funkcja wstrzymujaca proces macierzysty
        void wait(boost::shared_ptr<sop::process::Process>, boost::shared_ptr<sop::process::Process>);
        //funkcja zabijajaca proces
        void kill(boost::shared_ptr<sop::process::Process>);
        //funkcja zamykajaca proces
        void exit(boost::shared_ptr<sop::process::Process>);
        //definicja kolejki PIDow
        std::queue <uint16_t> PIDlist;
        //funkcja wype³niaj¹ca kolejke PIDami
        void fillQueue();
        //funkcja ustawiajaca element pierwszy z kolejki
        uint16_t getPIDfromList();


      
      protected:

      private:

    };
  }
}

#endif