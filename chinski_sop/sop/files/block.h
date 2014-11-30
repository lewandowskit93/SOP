#ifndef _SOP_FILES_BLOCK_H_
#define _SOP_FILES_BLOCK_H_

#pragma once

#include <vector>
#include <array>
#include <map>
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
      virtual std::vector<std::string> listDir() = 0;
      virtual void addInDir(std::string fileName, uint32_t blockAddress) = 0;
      virtual bool getIsDirectory() = 0;
      virtual void toggleLock() = 0;
      virtual void writeToFile(std::string, std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive) = 0;
      virtual uint32_t getAddress(std::string name) = 0;
      virtual void removeFile(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive) = 0;
      virtual void removeDir(std::vector<uint32_t>* freeSpace, std::array<Block*, sop::files::ConstEV::numOfBlocks>* drive) = 0;

    protected:

    private:

    };
  }
}

#endif