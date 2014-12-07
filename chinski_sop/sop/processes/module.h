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
        void addToVector(boost::shared_ptr<sop::process::Process>);
        void removeFromVector();
        void showObjectInList(uint16_t PID);
        boost::shared_ptr<sop::process::Process> findProcess(uint16_t PID);

      protected:

      private:

    };
  }
}

#endif