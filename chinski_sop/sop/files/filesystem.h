#ifndef _SOP_FILES_FILESYSTEM_H_
#define _SOP_FILES_FILESYSTEM_H_

#include <string>
#include <cstdint>
#include <vector>
#include "file.h"
#include "inode.h"

namespace sop
{
  namespace files
  {
    struct currentDirectory;
    class Filesystem;

    /*
      Filesystem class
      The place where all the witchcraft happens.
      Also that's the layer of communication between OS-layers.
    */
    class Filesystem
    {
    public:
      // Files
      File* openFile(pid* PID, std::string fileName, std::string subdirectory = "");
      std::string readFile(File* fileHandler);
      std::string* createFile(pid* PID, std::string fileName);
      uint16_t saveFile(File* fileHandler);
      uint16_t closeFile(File* fileHandler);
      uint16_t removeFile(pid* PID, std::string fileName);
      uint16_t renameFile(pid* PID, std::string fileName, std::string newFileName);
      uint16_t moveFile(pid* PID, std::string fileName, std::string newDirectory);
      uint16_t appendToFile(File* fileHandler, std::string data);
      File* seekForFile(pid* PID, std::string fileName);
      Attr* getAttributes(File* fileHandler);
      uint16_t setAttributes(File* fileHandler, Attr& setData);

      // Directories
      std::string getcurrentDir();
      uint16_t changeDirectory(pid* PID, std::string directoryName);
      uint16_t createDirectory(pid* PID, std::string newDirectoryName);
      uint16_t removeDirectory(pid* PID, std::string directoryName); //remember to delete all files and return them to freeSpaceVector
      uint16_t renameDirectory(pid* PID, std::string directoryName, std::string newDirectoryName);

      // Overall
      Arwx* list();
    protected:
    private:
      currentDirectory currentDir;
      Inode* rootDirectory;
      std::vector<uint8_t> freeSpaceVector;
    };


    /*
      Current opened directory containter
    */
    struct currentDirectory
    {
      uint32_t block;
      std::string name;
    };

    /*
      RWX list container
    */
    struct Arwx
    {
      std::string drwx;
      std::string name;
    };
  }
}

#endif