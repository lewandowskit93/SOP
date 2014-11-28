#ifndef _SOP_FILES_EXT_H_
#define _SOP_FILES_EXT_H_

#include <string>
#include <vector>
#include <cstdint>
#include <boost\shared_ptr.hpp>
#include "block.h"
#include "inode.h"

namespace sop
{
  namespace files
  {
    /*
      Ext class is a EXT2-like partition system to be used with VFS-like Filesystem class
    */
    class Ext
    {
    public:
      Ext();
      Ext(std::string driveFile);
      ~Ext();

    protected:

    private:
      std::vector<uint32_t> freeSpace;
      boost::shared_ptr<Block[]> dataBlocks;
    };
  }
}

#endif