#ifndef _SOP_SYSTEM_EXCEPTIONS_SYSTEM_EXCEPTIONS_H_
#define _SOP_SYSTEM_EXCEPTIONS_SYSTEM_EXCEPTIONS_H_

#include <string>
#include ".\sop\exception.h"

namespace sop
{
  namespace system
  {
    namespace exceptions
    {

      /*
        Exception for system exceptions.
        Every system exception should inherit from it.
      */
      class SystemException : public sop::Exception
      {
        public:
          SystemException();
          explicit SystemException(const std::string & value);
          virtual ~SystemException();
          virtual std::string getClassName() const;

        protected:

        private:

      };
    }
  }
}

#endif