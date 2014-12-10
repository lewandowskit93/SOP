#ifndef _SOP_FILES_DATA_H_
#define _SOP_FILES_DATA_H_

#include <string>
#include <array>
#include <vector>
#include "block.h"
#include "filesystem.h"

namespace sop
{
  namespace files
  {
    /*
      Directory list output format
    */
    struct dirList;

    /*
      Data class stores file data blocks
    */
    class Data : public Block
    {
    public:
      Data();
      Data(std::string);
      ~Data();
      std::array<char, sop::files::ConstEV::blockSize> getData_d();
      std::vector<std::array<char, sop::files::ConstEV::blockSize>> getData_i(std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk){std::vector<std::array<char, sop::files::ConstEV::blockSize>> x; return x;}
      uid_t getUID(){ return -1; }
      gid_t getGID(){ return -1; }
      void setData(std::array<char, sop::files::ConstEV::blockSize> data);
      std::vector<sop::files::dirList> listDir(std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk){ std::vector<sop::files::dirList> x; return x;}
      void addInDir(std::string fileName, uint32_t blockAddress){}
      void removeFromDir(std::string fileName){}
      bool getIsDirectory(){ return 0; }
      void toggleLock(){}
      uint32_t getSize(){ return 0; }
      void writeToFile(std::string, std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive){}
      uint32_t getAddress(std::string name){ return 0; }
      void removeFile(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive){}
      void removeDir(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive){}
      sop::users::Permissions getPermissions(){ return sop::users::Permissions(); }
      bool getLock(){ return false; }

    protected:

    private:
      std::array<char, sop::files::ConstEV::blockSize> containter;

      friend class Filesystem;
    };
  }
}

#endif