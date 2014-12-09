#ifndef _SOP_FILES_CONSTEV_H
#define _SOP_FILES_CONSTEV_H

#include <cstdint>

namespace sop
{
  namespace files
  {
    // Temporary just to maintain compability
    typedef int pid_t;
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
    };

    class ConstEV
    {
    public:
      static const uint32_t directAddrBlock = 3;
      static const uint32_t blockSize = 32;
      static const uint32_t numOfBlocks = 64;

    protected:

    private:
    };
  }
}

#endif