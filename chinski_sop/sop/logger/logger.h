#ifndef _SOP_LOGGER_LOGGER_H_
#define _SOP_LOGGER_LOGGER_H_

#include <string>
#include ".\sop\object.h"

namespace sop{
  namespace logger{

    /*
      Used to manage logging.
    */
    class Logger : public sop::Object{
      public:
        Logger();
        virtual ~Logger();
        virtual std::string getClassName();

      protected:

      private:

    };
  }
}

#endif