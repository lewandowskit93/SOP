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
//#include "filesystem_s.h"
#include "./sop/logger/logger.h"
#include "./sop/files/serialize.h"

namespace sop
{
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
      //Filesystem(sop::logger::Logger* logger);
      Filesystem(sop::logger::Logger* logger, std::string diskFileName);
      ~Filesystem();
      sop::logger::Logger* logger;
      std::string writeInode(uint32_t);
      std::string writeData(uint32_t);
      void readInode(uint32_t, std::vector<std::string>);
      void readData(uint32_t, std::string);
      void format();

      // Files
      File* openFile(sop::process::Process* PID, std::vector<std::string> path, std::string openMode);
      std::string readFile(File* fileHandler);
      void createFile(sop::process::Process* PID, std::vector<std::string> path);
      void saveFile(File* fileHandler);
      void closeFile(File* fileHandler);
      void removeFile(sop::process::Process* PID, std::vector<std::string> path);
      //uint16_t renameFile(sop::process::Process* PID, std::string fileName, std::string newFileName);
      void moveFile(sop::process::Process* PID, std::string fileName, std::string newDirectory);
      void writeToFile(File* fileHandler, std::string data);
      File* seek(sop::process::Process* PID, std::vector<std::string> path);
      // Attr* getAttributes(File* fileHandler);
      // uint16_t setAttributes(File* fileHandler, Attr& setData);

      // Directories
      std::string getCurrentDir();
      std::string getCurrentPath();
      void changeDirectory(sop::process::Process* PID, std::vector<std::string> path);
      void changeDirectoryUp();
      void createDirectory(sop::process::Process* PID, std::vector<std::string> path);
      void removeDirectory(sop::process::Process* PID, std::vector<std::string> path);
      //uint16_t renameDirectory(sop::process::Process* PID, std::string directoryName, std::string newDirectoryName);

      // Overall
      std::vector<dirList> list();
      void printStats();
      void printDisk(uint32_t parts);
      void printDiskTree(uint32_t depth);
      void diskTree(uint32_t depth, std::vector<sop::files::dirList> root, std::string prelimiter);
      void printDataBlock(uint32_t block);
      void printInodeBlock(uint32_t block);
      void writeToFileP(File* file_ptr, std::string input);

      // Handlers
      void changeDirectoryHandler(const std::vector<const std::string> & params);
      void moveHandler(const std::vector<const std::string> & params);
      void removeFileHandler(const std::vector<const std::string> & params);
      void viHandler(const std::vector<const std::string> & params);
      void createFileHandler(const std::vector<const std::string> & params);
      void createDirectoryHandler(const std::vector<const std::string> & params);
      void removeDirectoryHandler(const std::vector<const std::string> & params);
      //void seekHandler(const std::vector<const std::string> & params);
      void catHandler(const std::vector<const std::string> & params);
      void listHandler(const std::vector<const std::string> & params);
      void echoHandler(const std::vector<const std::string> & params);
      void statHandler(const std::vector<const std::string> & params);
      void formatHandler(const std::vector<const std::string> & params);
      void test(const std::vector<const std::string> & params);
    
    protected:

    private:
      CurrentDirectory currentDir;
      std::list<File*> openedFilesList;
      std::vector<uint32_t> freeSpace;
      std::array<Block*, sop::files::ConstEV::numOfBlocks> dataBlocks;
      uint32_t getCurrentPathIterator();
      Serialize* serialize;
      void vi(File*);

      friend class Filesystem_s;
    };
  }
}

#endif