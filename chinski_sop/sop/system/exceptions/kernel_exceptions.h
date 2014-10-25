#ifndef _SOP_SYSTEM_EXCEPTIONS_KERNEL_EXCEPTIONS_H_
#define _SOP_SYSTEM_EXCEPTIONS_KERNEL_EXCEPTIONS_H_

#include <string>
#include ".\sop\exception.h"

namespace sop
{
  namespace system
  {
    namespace exceptions
    {

      /*
        Exception for kernel exceptions.
        Every kernel exception should inherit from it.
      */
      class KernelException : public sop::Exception
      {
        public:
          KernelException();
          explicit KernelException(const std::string & value);
          virtual ~KernelException();
          virtual std::string getClassName() const;

        protected:

        private:

      };

      /*
        Exception thrown when kernel initialization fails.
      */
      class KernelInitializationException : public KernelException
      {
        public:
          KernelInitializationException();
          explicit KernelInitializationException(const std::string & value);
          virtual ~KernelInitializationException();
          virtual std::string getClassName() const;

        protected:

        private:

      };

     /*
        Exception thrown when kernel run fails.
      */
      class KernelRunException : public KernelException
      {
        public:
          KernelRunException();
          explicit KernelRunException(const std::string & value);
          virtual ~KernelRunException();
          virtual std::string getClassName() const;

        protected:

        private:

      };

      /*
        Exception thrown when kernel shutdown fails.
      */
      class KernelShutdownException : public KernelException
      {
        public:
          KernelShutdownException();
          explicit KernelShutdownException(const std::string & value);
          virtual ~KernelShutdownException();
          virtual std::string getClassName() const;

        protected:

        private:

      };
    }
  }
}

#endif