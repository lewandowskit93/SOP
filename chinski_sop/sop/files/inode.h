#ifndef _SOP_FILES_INODE_H_
#define _SOP_FILES_INODE_H_

#include <string>
#include <cstdint>
#include <vector>
#include <array>
#include <map>
#include "block.h"
#include "constev.h"
#include "inode_s.h"

namespace sop
{
  namespace files
  {
    /*
      Directory list output format
    */
    struct dirList;
    /*
      Directory helper structure
    */
    struct dir_u
    {
      std::map<std::string,uint32_t> inodesInside;
    };

    /*
      File helper structure
    */
    struct file_u
    {
      std::array<uint32_t, sop::files::ConstEV::directAddrBlock> directBlockAddr;
      std::vector<uint32_t> indirectBlockAddr;
      uint32_t size; // WRITE DYNAMIC SIZE
    };

    /*
      Inode class
      Don't touch, it's not interesting for you!
    */
    class Inode : public Block
    {
    public:
      Inode(bool isDirectory, uid_t UID, gid_t GID, sop::logger::Logger* logger);
      ~Inode();
      uid_t getUID();
      gid_t getGID();
      std::vector<std::array<char, sop::files::ConstEV::blockSize>> getData_i(std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk);
      std::array<char, sop::files::ConstEV::blockSize> getData_d(){ return *new std::array<char,sop::files::ConstEV::blockSize>; }
      std::vector<dirList> listDir(std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk);
      void addInDir(std::string fileName, uint32_t blockAddress);
      void removeFromDir(std::string fileName);
      bool getIsDirectory();
      void toggleLock();
      uint32_t getSize();
      void writeToFile(std::string, std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive);
      void removeFile(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive);
      void removeDir(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive);
      uint32_t getAddress(std::string name);

    protected:

    private:
      bool isDirectory;
      uint32_t uid;
      uint32_t gid;
      dir_u directory;
      file_u file;
      bool lock;
      sop::logger::Logger* logger;

      friend class Inode_s;
    };
  }
}

#endif