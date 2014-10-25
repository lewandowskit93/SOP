#ifndef _SOP_LOGGER_LOGGER_H_
#define _SOP_LOGGER_LOGGER_H_

#include <string>
#include <cstdint>
#include ".\sop\object.h"

namespace sop
{
  namespace logger
  {

    /*
      Abstract class used to manage logging.
      Real loggers should inherit from it and implement saveLog function.
    */
    class Logger : public sop::Object
    {
      public:
        static class Channel
        {
          public:
            static const uint16_t USERS_CHANNEL = 1;
            static const uint16_t FILES_CHANNEL = 2;
            static const uint16_t PROCESSES_CHANNEL = 3;
            static const uint16_t MEMORY_CHANNEL = 4;
            static const uint16_t PROCESSOR_CHANNEL = 5;

            /*
              Returns string name of given log channel.
              If channel doesnt have name then the given value is returned.
            */
            static std::string getChannelName(uint16_t channel);

          private:
            Channel();
        };



        static class Level
        {
          public:
            static const uint16_t OFF = 0;
            static const uint16_t SEVERE = 1;
            static const uint16_t WARNING = 2;
            static const uint16_t INFO = 3;
            static const uint16_t CONFIG = 4;
            static const uint16_t FINE = 5;
            static const uint16_t FINER = 6;
            static const uint16_t FINEST = 7;

            /*
              Returns string name of given log level.
              If level doesnt have name then the given value is returned.
            */
            static std::string getLevelName(uint16_t level);

          private:
            Level();
        };

        /*
          Creates logger which logs only SEVERE logs.
        */
        Logger();

        /*
          Creates logger which logs logs with level <= logging_level.
        */
        explicit Logger(uint16_t logging_level);
        virtual ~Logger();
        virtual std::string getClassName() const;

        /*
          Logs a message with given channel and level.
        */
        void log(uint16_t  channel, uint16_t level, const std::string & message);

        /*
          Logs a message with given level on USERS_CHANNEL.
        */
        void logUsers(uint16_t level, const std::string & message);

        /*
          Logs a message with given level on FILES_CHANNEL.
        */
        void logFiles(uint16_t level, const std::string & message);

        /*
          Logs a message with given level on PROCESSES_CHANNEL.
        */
        void logProcesses(uint16_t level, const std::string & message);

        /*
          Logs a message with given level on MEMORY_CHANNEL.
        */
        void logMemory(uint16_t level, const std::string & message);

        /*
          Logs a message with given level on PROCESSOR_CHANNEL.
        */
        void logProcessor(uint16_t level, const std::string & message);

        /*
          Returns current logging level.
        */
        uint16_t getLoggingLevel() const;

        /*
          Sets logging level.
        */
        void setLoggingLevel(uint16_t level);

      protected:
        /*
          Should save log (e.g. to file, to network, to console).
          This function is invoked only when logger is logging logs with given level.
        */
        virtual void saveLog(uint16_t  channel, uint16_t level, const std::string & message) = 0;

      private:
        /*
          Returns true if log with specified level can be logged or false otherwise.
        */
        bool canLog(uint16_t level);

        /*
          Logger logs every log with level <= _logging_level.
        */
        uint16_t _logging_level;

    };
  }
}

#endif