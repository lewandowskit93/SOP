#ifndef _SOP_FILES_CONSTEV_H
#define _SOP_FILES_CONSTEV_H

#include <cstdint>
#include "./sop/object.h"

namespace sop
{
  namespace files
  {
    // Temporary just to maintain compabilitysop::process::Process
    typedef int uid_t;
    typedef int gid_t;
    typedef bool lock_t;
    typedef int addr_t;

    /*
      Directory list output format
    */
    struct dirList
    {
      std::string drwx;
      std::string size;
      std::string name;
      uint32_t block;
      uint32_t username;
      uint32_t group;
    };

    class ConstEV
    {
    public:
      // Sets inode direct block addresses
      static const uint32_t directAddrBlock = 3;

      // Sets constant block size
      static const uint32_t blockSize = 32;

      // Sets number of blocks on drive
      static const uint32_t numOfBlocks = 128;

    protected:

    private:
    };
  }
}

#endif