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

      /*
        Exception thrown when system initialization fails.
      */
      class SystemInitializationException : public SystemException
      {
        public:
          SystemInitializationException();
          explicit SystemInitializationException(const std::string & value);
          virtual ~SystemInitializationException();
          virtual std::string getClassName() const;

        protected:

        private:

      };

     /*
        Exception thrown when system run fails.
      */
      class SystemRunException : public SystemException
      {
        public:
          SystemRunException();
          explicit SystemRunException(const std::string & value);
          virtual ~SystemRunException();
          virtual std::string getClassName() const;

        protected:

        private:

      };

      /*
        Exception thrown when system shutdown fails.
      */
      class SystemShutdownException : public SystemException
      {
        public:
          SystemShutdownException();
          explicit SystemShutdownException(const std::string & value);
          virtual ~SystemShutdownException();
          virtual std::string getClassName() const;

        protected:

        private:

      };
    }
  }
}

#endif