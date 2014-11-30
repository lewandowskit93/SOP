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

namespace sop
{
  namespace files
  {
    class File;
    class Ext;
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
      Filesystem();
      Filesystem(std::string diskFileName);
      ~Filesystem();

      // Files
      File* openFile(pid_t* PID, std::vector<std::string> path, char openMode);
      std::string readFile(File* fileHandler);
      void createFile(pid_t* PID, std::vector<std::string> path);
      void saveFile(File* fileHandler);
      void closeFile(File* fileHandler);
      void removeFile(pid_t* PID, std::vector<std::string> path);
      //uint16_t renameFile(pid_t* PID, std::string fileName, std::string newFileName);
      void moveFile(pid_t* PID, std::string fileName, std::string newDirectory);
      void writeToFile(File* fileHandler, std::string data);
      File* seek(pid_t PID, std::vector<std::string> path);
      // Attr* getAttributes(File* fileHandler);
      // uint16_t setAttributes(File* fileHandler, Attr& setData);

      // Directories
      std::string getCurrentDir();
      std::string getCurrentPath();
      void changeDirectory(pid_t* PID, std::string directoryName);
      void changeDirectoryUp();
      void createDirectory(pid_t* PID, std::string newDirectoryName);
      void removeDirectory(pid_t* PID, std::string directoryName); // remember to delete all files and return them to freeSpaceVector
      //uint16_t renameDirectory(pid_t* PID, std::string directoryName, std::string newDirectoryName);

      // Overall
      std::vector<std::string> list();
      void printStats();
      void printDisk(uint32_t parts);

      // Handlers
      void changeDirectoryHandler(const std::vector<const std::string> & params);
      void moveHandler(const std::vector<const std::string> & params);
      void removeFileHandler(const std::vector<const std::string> & params);
      void nanoHandler(const std::vector<const std::string> & params);
      void createFileHandler(const std::vector<const std::string> & params);
      void createDirectoryHandler(const std::vector<const std::string> & params);
      void removeDirectoryHandler(const std::vector<const std::string> & params);
      //void seekHandler(const std::vector<const std::string> & params);
      void catHandler(const std::vector<const std::string> & params);
      void listHandler(const std::vector<const std::string> & params);
      void echoHandler(const std::vector<const std::string> & params);
    protected:

    private:
      CurrentDirectory currentDir;
      std::list<File*> openedFilesList;
      std::vector<uint32_t> freeSpace;
      std::array<Block*, sop::files::ConstEV::numOfBlocks> dataBlocks;
    };
  }
}

#endif