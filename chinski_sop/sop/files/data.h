#ifndef _SOP_FILES_DATA_H_
#define _SOP_FILES_DATA_H_

#include <string>
#include <array>
#include <vector>
#include "block.h"

namespace sop
{
  namespace files
  {
    //class Block;

    class Data : public Block
    {
    public:
      Data();
      ~Data();
      std::array<char, sop::files::ConstEV::blockSize> getData_d();
      //std::vector<std::array<char, sop::files::ConstEV::blockSize>> getData_i(){}
      uid_t getUID(){ return 0; }
      gid_t getGID(){ return 0; }
      void setData(std::array<char, sop::files::ConstEV::blockSize> data);

    protected:

    private:
      std::array<char, sop::files::ConstEV::blockSize> containter;
    };
  }
}

#endif