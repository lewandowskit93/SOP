#ifndef _SOP_FILES_FILE_H_
#define _SOP_FILES_FILE_H_

#include <string>

namespace sop
{
  namespace files
  {
    // Temporary just to maintain compability
    typedef uint32_t pid;

    /*
      File class
      Integrative containter which have the access to whole data of one file.
    */
    class File
    {
    public:

    protected:
      pid* PIDHolder;
    private:
      Perm* permissions;
    };

    /*
      Attributes container
    */
    struct Attr
    {

    };

    /*
      Permission container
    */
    struct Perm
    {
      bool readPermission;
      bool writePermission;
    };
  }
}

#endif