#ifndef _SOP_FILES_INODE_S_H_
#define _SOP_FILES_INODE_S_H_

#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>
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

      void readBlock(std::string);
      std::string writeBlock();

    protected:

    private:
      Inode* inode_p;
      bool isDirectory;
      uint32_t uid;
      uint32_t gid;

      // File
      std::array<uint32_t, sop::files::ConstEV::directAddrBlock> directBl;
      std::vector<uint32_t> indirectBl;
      uint32_t size;

      //Dir
      std::map<std::string,uint32_t> dirMap;

      void readDirectory(std::vector<std::string>);
      std::string writeDirectory();
      void readFile(std::vector<std::string>);
      std::string writeFile();

      void assign();
      void itemize(std::string data);
      std::string serialize();
    };
  }
}

#endif