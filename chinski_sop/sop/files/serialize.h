#ifndef _SOP_FILES_SERIALIZE_H_
#define _SOP_FILES_SERIALIZE_H_

#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include "sop/files/filesystem.h"
#include "sop/files/filesystem_s.h"
#include "sop/logger/logger.h"

namespace sop
{
  namespace files
  {
    class Serialize
    {
    public:
      Serialize(Filesystem* fsPtr, std::string filename, sop::logger::Logger* logger);
      ~Serialize();

      void save();
      void read();
    protected:
    private:
      boost::shared_ptr<Filesystem> filesystem_p;
      boost::shared_ptr<Filesystem_s> fsSerialize_p;
      std::string filename;
      sop::logger::Logger* logger;
    };
  }
}

#endif