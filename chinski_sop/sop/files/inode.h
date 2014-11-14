#ifndef _SOP_FILES_INODE_H_
#define _SOP_FILES_INODE_H_

#include <string>
#include "block.h"

namespace sop
{
  namespace files
  {
    /*
      Inode class
      Don't touch, it's not interesting for you!
    */
    class Inode : public Block
    {
    public:

    protected:

    private:
      // Remember 'bout the lock flag!!!
    };
  }
}

#endif