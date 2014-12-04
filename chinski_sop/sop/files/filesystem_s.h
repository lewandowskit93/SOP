#ifndef _SOP_FILES_FILESYSTEM_S_H_
#define _SOP_FILES_FILESYSTEM_S_H_

#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include "sop/files/filesystem.h"
#include "sop/files/inode_s.h"
#include "sop/files/data_s.h"
#include "sop/logger/logger.h"

namespace sop
{
  namespace files
  {
    class Filesystem_s
    {
    public:
      Filesystem_s(Filesystem* fpPtr, sop::logger::Logger* logger);
      ~Filesystem_s();

      void readData();
      void writeData();

    protected:

    private:
      boost::shared_ptr<Filesystem> filesystem_p;
        
      void readFreeSpace();
      void writeFreeSpace();
      void readDataBlocks();
      void writeDataBlocks();
      sop::logger::Logger* logger;
    };
  }
}
#endif