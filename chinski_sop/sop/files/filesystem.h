#ifndef _SOP_FILES_FILESYSTEM_H_
#define _SOP_FILES_FILESYSTEM_H_

#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include <list>
#include <map>
#include "file.h"
#include "inode.h"

namespace sop
{
  namespace files
  {

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
      ~Filesystem();

      // Files
      File* openFile(pid_t* PID, std::string fileName, std::string openMode ,std::string subdirectory = "");
      std::string readFile(File* fileHandler);
      std::string* createFile(pid_t* PID, std::string fileName);
      uint16_t saveFile(File* fileHandler);
      uint16_t closeFile(File* fileHandler);
      uint16_t removeFile(pid_t* PID, std::string fileName);
      uint16_t renameFile(pid_t* PID, std::string fileName, std::string newFileName);
      uint16_t moveFile(pid_t* PID, std::string fileName, std::string newDirectory);
      uint16_t appendToFile(File* fileHandler, std::string data);
      File* seekForFile(pid_t* PID, std::string fileName);
      // Attr* getAttributes(File* fileHandler);
      // uint16_t setAttributes(File* fileHandler, Attr& setData);

      // Directories
      std::string getcurrentDir();
      uint16_t changeDirectory(pid_t* PID, std::string directoryName);
      uint16_t createDirectory(pid_t* PID, std::string newDirectoryName);
      uint16_t removeDirectory(pid_t* PID, std::string directoryName); // remember to delete all files and return them to freeSpaceVector
      uint16_t renameDirectory(pid_t* PID, std::string directoryName, std::string newDirectoryName);

      // Overall
      std::vector<std::map<std::string, std::string>> list(); // map<drwx - name>

      // Handlers
      void changeDirectoryHandler(const std::vector<const std::string> & params);
    protected:

    private:
      CurrentDirectory currentDir;
      Inode rootDirectory;
      std::vector<uint8_t> freeSpaceVector;
      std::list<File> openedFilesList;
    };
  }
}

#endif