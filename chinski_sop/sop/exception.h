#ifndef _SOP_EXCEPTION_H_
#define _SOP_EXCEPTION_H_

#include <string>
#include ".\sop\object.h"

namespace sop
{

  /*
    Simple Exception class.
    Every exception class should directly or indirectly inherit from it.
  */
  class Exception : public sop::Object
  {
    public:
      Exception();
      explicit Exception(const std::string & value);
      virtual ~Exception();
      virtual std::string getClassName() const;
      
      /*
        Returns exception value.
      */
      virtual std::string getValue() const;
      virtual std::string toString() const;

    protected:

    private:
      std::string _value;
  };
}

#endif