#ifndef _SOP_FILES_FILESYSTEM_H_
#define _SOP_FILES_FILESYSTEM_H_

#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include <list>
#include <map>
#include "file.h"
#include "block.h"
#include <boost\shared_ptr.hpp>
//#include "filesystem_s.h"
#include "./sop/logger/logger.h"
#include "./sop/files/serialize.h"
//#include "./sop/system/kernel.h"

std::vector<std::string> getPathFromParam(std::string path);

namespace sop
{
  namespace system
  {
    class Kernel;
  }

  namespace files
  {
    class File;
    class Ext;
    class Serialize;
    /*
      Current opened directory containter
    */
    struct CurrentDirectory
    {
      std::vector<uint32_t> blockRoute;
      std::vector<std::string> path;
    };

    /*
      Filesystem class
      The place where all the witchcraft happens.
      Also that's the layer of communication between OS-layers.
    */
    class Filesystem
    {
    public:
      Filesystem(sop::system::Kernel *kernel,sop::logger::Logger* _logger, std::string diskFileName);
      ~Filesystem();

      // Creates empty disk
      void format();

      ///////////
      // Files //
      ///////////

      // Opens file and adds it to the vector of opened files
      File* openFile(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path, std::string openMode);

      // Reads file inside fileHandler pointer and returns it as a string
      std::string readFile(File* fileHandler);

      // Creates new file
      void createFile(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path);

      // Saves file
      void saveFile(File* fileHandler);

      // Closes file and removes it from opened files vector
      void closeFile(File* fileHandler);

      // Removes file on disk
      void removeFile(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path);

      // Writes to file data inside string
      void writeToFile(File* fileHandler, std::string data);

      // Searches for file in path vector
      // Returns nullptr if not found
      File* seek(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path);

      /////////////////
      // Directories //
      /////////////////

      // Returns current opened directory
      std::string getCurrentDir();

      // Returns current opened path
      std::string getCurrentPath();

      // Changes directory as in path
      void changeDirectory(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path);

      // Shorthand to change one directory upwards
      void changeDirectoryUp();

      // Creates new directory as in path
      void createDirectory(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path);

      // Removes directory as in path
      void removeDirectory(boost::shared_ptr<sop::process::Process> PID, std::vector<std::string> path);

      /////////////
      // Overall //
      /////////////

      // Retunrs vector of structures from current directory
      std::vector<dirList> list();

      // Prints filesystem statistics
      void printStats();

      // Prints disk statistics
      void printDisk(uint32_t parts);

      // Prints disk tree
      void printDiskTree(uint32_t depth);

      // Searches the disk tree and prints using printDiskTree
      void diskTree(uint32_t depth, std::vector<sop::files::dirList> root, std::string prelimiter);

      // Prints data block statistics
      void printDataBlock(uint32_t block);

      // Prints inode block statistics
      void printInodeBlock(uint32_t block);

      // Writes to file pointed in file pointer data inside string
      void writeToFileP(File* file_ptr, std::string input);

      //////////////
      // Handlers //
      //////////////

      // Shell handler changing directory as stated in parameters
      void changeDirectoryHandler(const std::vector<const std::string> & params);

      // Shell handler removing file as stated in parameters
      void removeFileHandler(const std::vector<const std::string> & params);

      // Shell handler for vi-like file editor
      void viHandler(const std::vector<const std::string> & params);

      // Shell handler creating new file as stated in parameters
      void createFileHandler(const std::vector<const std::string> & params);

      // Shell handler creating new directory as stated in parameters
      void createDirectoryHandler(const std::vector<const std::string> & params);

      // Shell handler removing directory as stated in parameters
      void removeDirectoryHandler(const std::vector<const std::string> & params);

      // Shell handler for reading and printing file as stated in parameters
      void catHandler(const std::vector<const std::string> & params);

      // Shell handler for listing and printing directory as stated in parameters
      void listHandler(const std::vector<const std::string> & params);

      // Shell handler for printing statistics as stated in parameters
      void statHandler(const std::vector<const std::string> & params);

      // Shell handler for creating new  as stated in parameters
      void formatHandler(const std::vector<const std::string> & params);
    
    protected:

    private:
      // Current directory holder
      CurrentDirectory currentDir;

      // List of opened files holder
      std::list<File*> openedFilesList;

      // Vector of free spaces on disk
      std::vector<uint32_t> freeSpace;

      // Actual disk blocks
      std::array<Block*, sop::files::ConstEV::numOfBlocks> dataBlocks;

      // Logger holder
      sop::logger::Logger* logger;

      // Filesystem serialization module holder
      Serialize* serialize;

      // Returns last block on current directory path
      uint32_t getCurrentPathIterator();
      
      // Vi-like file editor
      void vi(File*);

      // Serialization module read inode
      void readInode(uint32_t, std::vector<std::string>);

      // Serialization module read data
      void readData(uint32_t, std::string);

      // Serialization write inode
      std::string writeInode(uint32_t);

      // Serializtion write data
      std::string writeData(uint32_t);

      // Kernel holder
      sop::system::Kernel* _kernel;

      friend class Filesystem_s;
    };
  }
}

#endif