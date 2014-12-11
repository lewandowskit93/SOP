#ifndef _SOP_FILES_FILE_H_
#define _SOP_FILES_FILE_H_

#include <string>
#include <cstdint>
#include <vector>
#include <boost\shared_ptr.hpp>
#include <array>
#include "inode.h"
#include "filesystem.h"
#include "constev.h"
#include ".\sop\temporary.h"
#include ".\sop\users\id_definitions.h"
#include ".\sop\users\permissions.h"
#include ".\sop\users\permissions_manager.h"

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
      File(boost::shared_ptr<sop::process::Process> PID, uint32_t parentCatalog, uint32_t blockAddress, std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk, sop::logger::Logger* logger);
      ~File();

      // Returns block address of file
      uint32_t getBlockAddr();

      // Returns data in file
      std::vector<std::array<char, sop::files::ConstEV::blockSize>> getData();

      // Returns current mode in which the file is opened 
      char getMode();

      // Returns filename of this file
      std::string getFileName();

      // Returns UID
      uid_t getUID();

      // Returns GID
      gid_t getGID();

      // Writes to file data in string
      void writeToFile(std::string, std::vector<uint32_t>* freeSpace);

      // Removes curenlty opened file
      void sop::files::File::removeFile(std::vector<uint32_t>* freeSpace);

      // Sets mode in which this file is opened
      void setMode(char mode);

      // Returns Inode data of current file (for user management reasons)
      Inode* getInode();

      // Return file size
      uint32_t getSize();

      // Sets the filename of current file as data in string
      void setFilename(std::string);

      // Returns Process identification
      boost::shared_ptr<sop::process::Process> getPID();

      // Returns permissions (for user management reasons)
      sop::users::Permissions getPermission();

    protected:
      // Holder of the opening process identification
      boost::shared_ptr<sop::process::Process> PIDHolder;

    private:
      // Parent directory address holder
      uint32_t parentCatalogAddress;

      // This inode block address holder
      uint32_t blockAddress;

      // Data holder
      std::vector<std::array<char, sop::files::ConstEV::blockSize>> data;

      // Data load ckecker (currenty irrelevant due to data serialization)
      bool isDataLoaded;

      // Actual mode in which the file is opened (rwx)
      char openMode; 

      // Loads data from inode to data holder in this structure
      void loadData();

      // Drive holder
      std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive;

      ////////////////
      // Attributes //
      ////////////////

      // Filename holder
      std::string fileName;

      // UID holder
      uid_t UID;

      // GID holder
      gid_t GID;
      
      // Logger holder
      sop::logger::Logger* logger;
    };
  }
}

#endif