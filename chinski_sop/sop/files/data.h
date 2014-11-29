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
    /*
      Data class stores file data blocks
    */
    class Data : public Block
    {
    public:
      Data();
      ~Data();
      std::array<char, sop::files::ConstEV::blockSize> getData_d();
      std::vector<std::array<char, sop::files::ConstEV::blockSize>> getData_i(){std::vector<std::array<char, sop::files::ConstEV::blockSize>> x; return x;}
      uid_t getUID(){ return -1; }
      gid_t getGID(){ return -1; }
      void setData(std::array<char, sop::files::ConstEV::blockSize> data);
      std::vector<std::string> listDir(){ throw -1; }
      void addInDir(std::string fileName, uint32_t blockAddress){ throw -1; }

    protected:

    private:
      std::array<char, sop::files::ConstEV::blockSize> containter;
    };
  }
}

#endif