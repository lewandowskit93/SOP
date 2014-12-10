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
      // Filesystem holder
      Filesystem* filesystem_p;

      // Serialization module of filesystem holder
      Filesystem_s* fsSerialize_p;

      // Drive-file filename holder
      std::string filename;

      // Logger holder
      sop::logger::Logger* logger;

      // Turns filesystem from objects to stringstrem and sends it to fstream parameter
      // For every block of data conversion is made to transport data from object to stringstream
      // Free spaces as saved as vector of data
      // All fields have labels to maintain readability
      void serialize(std::fstream&);

      // Reads fstream and turns it into objects(items)
      void itemize(std::fstream&);

      // Filesystem serialization module holder
      Filesystem_s* serialFilesystem;
    };
  }
}

#endif