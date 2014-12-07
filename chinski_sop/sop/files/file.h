#ifndef _SOP_FILES_FILE_H_
#define _SOP_FILES_FILE_H_

#include <string>
#include <cstdint>
#include <vector>
#include <array>
#include "inode.h"
#include "filesystem.h"
#include "constev.h"

namespace sop
{
  namespace files
  {
    class Filesystem;
     /*
      File class
      Integrative containter which have the access to whole data of one file.
    */
    class File
    {
    public:
      File(pid_t* PID, uint32_t parentCatalog, uint32_t blockAddress, std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk, sop::logger::Logger* logger);
      ~File();

      uint32_t getBlockAddr();
      std::vector<std::array<char, sop::files::ConstEV::blockSize>> getData();
      char getMode();
      std::string getFileName();
      uid_t getUID();
      gid_t getGID();
      void writeToFile(std::string, std::vector<uint32_t>* freeSpace);
      void sop::files::File::removeFile(std::vector<uint32_t>* freeSpace);
      void setMode(char mode);
      uint32_t getSize();
      void setFilename(std::string);

    protected:
      pid_t* PIDHolder;

    private:
      uint32_t parentCatalogAddress;
      uint32_t blockAddress;
      std::vector<std::array<char, sop::files::ConstEV::blockSize>> data;
      bool isDataLoaded;
      char openMode; // Actual mode in which the file is opened (rwx)
      void loadData();
      std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive;

      // Attributes
      std::string fileName;
      uid_t UID;
      gid_t GID;

      sop::logger::Logger* logger;
    };
  }
}

#endif