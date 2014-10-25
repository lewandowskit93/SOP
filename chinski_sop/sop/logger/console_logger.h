#ifndef _SOP_LOGGER_CONSOLE_LOGGER_H_
#define _SOP_LOGGER_CONSOLE_LOGGER_H_

#include <string>
#include <cstdint>
#include ".\sop\logger\logger.h"

namespace sop
{
  namespace logger
  {
    /*
      Simple logger that saves logs to standard output stream.
    */
    class ConsoleLogger : public Logger
    {
      public:
        ConsoleLogger();
        explicit ConsoleLogger(uint16_t logging_level);
        virtual ~ConsoleLogger();
        virtual std::string getClassName();

      protected:
        virtual void saveLog(uint16_t channel, uint16_t level, const std::string & message);

      private:
    };
  }
}

#endif