#ifndef _SOP_FILES_INODE_H_
#define _SOP_FILES_INODE_H_

#include <string>
#include <cstdint>
#include <vector>
#include <array>
#include <map>
#include "block.h"
#include "constev.h"
//#include "filesystem_s.h"
#include ".\sop\logger\logger.h"
#include "temporary.h"

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

      // Returns Inode UID
      uid_t getUID();

      // Returns Inode GID
      gid_t getGID();

      // Returns data from file
      std::vector<std::array<char, sop::files::ConstEV::blockSize>> getData_i(std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk);

      // Not for inode use
      std::array<char, sop::files::ConstEV::blockSize> getData_d(){ return *new std::array<char,sop::files::ConstEV::blockSize>; }

      // Returns list of directories inside directory inode
      std::vector<dirList> listDir(std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk);

      // Inserts new Inode inside current
      void addInDir(std::string fileName, uint32_t blockAddress);

      // Removes child from current Inode
      void removeFromDir(std::string fileName);

      // Returns if this Inode is a directory
      bool getIsDirectory();

      // Changes state of Inode lock
      void toggleLock();

      // If file returns size of file
      // else returns 0
      uint32_t getSize();

      // If file writes data in string to data blocks
      // else undefined behavior
      void writeToFile(std::string, std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive);

      // Removes specified file
      void removeFile(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive);

      // Removes specified directory
      void removeDir(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive);
      
      // If directory returns address of file from directory table, if doesn't exists returns 0
      // else returns 0
      uint32_t getAddress(std::string name);

      // Returns permissions structure
      sop::users::Permissions getPermissions();

      // Returns inode lock
      bool getLock();

    protected:

    private:
      bool isDirectory;
      uint32_t uid;
      uint32_t gid;
      dir_u directory;
      file_u file;
      sop::users::Permissions permissions;
      bool lock;
      sop::logger::Logger* logger;

      friend class Filesystem;
    };
  }
}

#endif