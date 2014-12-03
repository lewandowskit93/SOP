#ifndef _SOP_FILES_INODE_S_H_
#define _SOP_FILES_INODE_S_H_

#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "sop/files/inode.h"

namespace sop
{
  namespace files
  {
    class Inode;
    class Inode_s
    {
    public:
      Inode_s(Inode* ptr);
      ~Inode_s();

      void readBlock();
      void writeBlocK();

    protected:

    private:
      boost::shared_ptr<Inode> inode_p;

      std::vector<char> readDirectory();
      void writeDirectory();
      std::vector<char> readFile();
      void writeFile();
    };
  }
}

#endif