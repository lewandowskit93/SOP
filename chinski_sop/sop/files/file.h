#ifndef _SOP_FILES_FILE_H_
#define _SOP_FILES_FILE_H_

#include <string>
#include "block.h"

namespace sop
{
  namespace files
  {
    // Temporary just to maintain compability
    typedef int pid_t;
    typedef int uid_t;
    typedef int gid_t;
    typedef bool lock_t;
    typedef int addr_t;

    /*
      File class
      Integrative containter which have the access to whole data of one file.
    */
    class File
    {
    public:

    protected:
      pid_t* PIDHolder;
    private:
      std::string* data;
      bool isDataLoaded;
      //permissions

      // Attributes
      std::string fileName;
      uid_t UID;
      gid_t GID;
      lock_t lock;

      // Data block addresses
      addr_t directAddr[3];
      addr_t* indirectAddr;
    };
  }
}

#endif