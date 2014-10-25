#ifndef _SOP_EXCEPTION_H_
#define _SOP_EXCEPTION_H_

#include <string>
#include ".\sop\object.h"

namespace sop{

  /*
    Simple Exception class.
    Every exception class should directly or indirectly inherit from it.
  */
  class Exception : public sop::Object{
    public:
      Exception();
      explicit Exception(const std::string & value);
      virtual ~Exception();
      virtual std::string getClassName();
      
      /*
        Returns exception value.
      */
      virtual std::string getValue();
      virtual std::string toString();

    protected:

    private:
      std::string _value;
  };
}

#endif