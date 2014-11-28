#ifndef _SOP_FILES_CONSTEV_H
#define _SOP_FILES_CONSTEV_H

#include <cstdint>

namespace sop
{
  namespace files
  {
    class ConstEV
    {
    public:
      static const uint32_t directBlockAddresses = 3;
      static const uint32_t blockSize = 32;
      static const uint32_t numOfBlocks = 64;

    protected:

    private:
    };
  }
}

#endif