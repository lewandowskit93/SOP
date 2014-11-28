#ifndef _SOP_FILES_BLOCK_H_
#define _SOP_FILES_BLOCK_H_

#pragma once

#include <vector>
#include <array>
#include "constev.h"

namespace sop
{
  namespace files
  {
    class Ext;
    /*
      Block class-interface is used to store all data on physical drive
    */
    class Block
    {
    public:
      //virtual ~Block();
      virtual uid_t getUID() = 0;
      virtual gid_t getGID() = 0;
      virtual std::vector<std::array<char, sop::files::ConstEV::blockSize>> getData_i(std::array<Block*, sop::files::ConstEV::numOfBlocks>* disk) = 0;
      virtual std::array<char, sop::files::ConstEV::blockSize> getData_d() = 0;

    protected:

    private:

    };
  }
}

#endif