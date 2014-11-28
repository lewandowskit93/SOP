#ifndef _SOP_FILES_INODE_H_
#define _SOP_FILES_INODE_H_

#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include "block.h"
#include "constev.h"

namespace sop
{
  namespace files
  {
    /*
      Directory helper structure
    */
    struct dir_u
    {
      std::vector<std::map<char[3],uint32_t>> inodesInside;
    };

    /*
      File helper structure
    */
    struct file_u
    {
      std::vector<uint32_t> directBlockAddr; //ConstEV.directBlockAddressess
      std::vector<uint32_t> indirectBlockAddr;
    };

    /*
      Inode class
      Don't touch, it's not interesting for you!
    */
    class Inode : public Block
    {
    public:
      Inode();
      ~Inode();
      uid_t getUID();
      gid_t getGID();
      std::vector<std::array<char, sop::files::ConstEV::blockSize>> getData_i(std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk);
      //std::array<char, sop::files::ConstEV::blockSize> getData_d(){ return auto(0);}

    protected:

    private:
      bool isDirectory;
      uint32_t uid;
      uint32_t gid;
      bool lock;
      dir_u directory;
      file_u file;
    };
  }
}

#endif