#ifndef _SOP_PROCESSOR_MODULE_H_
#define _SOP_PROCESSOR_MODULE_H_

#include <string>
#include ".\sop\system\module.h"
#include ".\sop\processor\executive_unit.h"


namespace sop
{
  namespace processor
  {
    /*
      Processor module class.
    */
    class Module : public sop::system::Module
    {
      public:
        explicit Module(sop::system::Kernel *kernel);
        virtual ~Module();
        virtual std::string getClassName() const;
        virtual void initialize();
        ExecutiveUnit *ExUnit;



      protected:

      private:

    };
  }
}

#endif