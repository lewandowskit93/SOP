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

      void readData(std::vector<std::string>);
      std::vector<std::string> writeData();
      void formatFS();

    protected:

    private:
      Filesystem* filesystem_p;
        
      void readFreeSpace(std::string);
      void readDataBlocks(std::vector<std::string>);
      std::string writeFreeSpace();
      std::vector<std::string> writeDataBlocks();
      sop::logger::Logger* logger;
    };
  }
}
#endif