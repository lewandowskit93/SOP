#ifndef _SOP_FILES_SERIALIZE_H_
#define _SOP_FILES_SERIALIZE_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "sop/files/filesystem.h"
#include "sop/files/filesystem_s.h"
#include "sop/logger/logger.h"

namespace sop
{
  namespace files
  {
    class Filesystem_s;
    class Filesystem;
    class Serialize
    {
    public:
      Serialize(Filesystem* fsPtr, std::string filename, sop::logger::Logger* logger);
      ~Serialize();

      void save();
      void read();
    protected:
    private:
      Filesystem* filesystem_p;
      Filesystem_s* fsSerialize_p;
      std::string filename;
      sop::logger::Logger* logger;
      void serialize(std::fstream&);
      void itemize(std::fstream&);
      Filesystem_s* serialFilesystem;
    };
  }
}

#endif