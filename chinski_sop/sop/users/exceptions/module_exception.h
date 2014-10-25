#ifndef _SOP_USERS_EXCEPTIONS_MODULE_EXCEPTION_H_
#define _SOP_USERS_EXCEPTIONS_MODULE_EXCEPTION_H_

#include <string>
#include ".\sop\exception.h"
#include ".\sop\system\exceptions\module_exceptions.h"

namespace sop
{
  namespace users
  {
    namespace exceptions
    {
      /*
        Simple Exception class for Users Module exceptions.
        Every users module exception class should directly or indirectly inherit from it.
      */
      class ModuleException : public sop::system::exceptions::ModuleException
      {
        public:
          ModuleException();
          explicit ModuleException(const std::string & value);
          virtual ~ModuleException();
          virtual std::string getClassName() const;

        protected:

        private:

      };
    }
  }
}

#endif