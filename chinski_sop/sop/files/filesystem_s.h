#ifndef _SOP_FILES_FILESYSTEM_S_H_
#define _SOP_FILES_FILESYSTEM_S_H_

#include <iostream>
#include <string>
#include "sop/files/filesystem.h"
//#include "sop/files/inode.h"
//#include "sop/files/data.h"
#include "sop/logger/logger.h"

namespace sop
{
  namespace files
  {
    class Filesystem;
    class Filesystem_s
    {
    public:
      Filesystem_s(Filesystem* fpPtr, sop::logger::Logger* logger);
      ~Filesystem_s();

      // Performs data reading from vector of strings 
      void readData(std::vector<std::string>);

      // Returns vector of strings from data
      std::vector<std::string> writeData();

      // Performs Filesystem format
      void formatFS();

    protected:

    private:
      // Filesystem base holder
      Filesystem* filesystem_p;
        
      // Helper method reading spaces from string
      void readFreeSpace(std::string);

      // Helper method reading blocks (inodes and data) from vector of strings
      void readDataBlocks(std::vector<std::string>);

      // Performs conversions from vector to string
      std::string writeFreeSpace();

      // Converts data blocks to vector of strings
      std::vector<std::string> writeDataBlocks();

      // Logger holder
      sop::logger::Logger* logger;
    };
  }
}
#endif