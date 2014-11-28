#ifndef _SOP_FILES_DATA_H_
#define _SOP_FILES_DATA_H_

#include <string>
#include <boost\shared_ptr.hpp>
#include "block.h"

namespace sop
{
  namespace files
  {
    class Data : public Block
    {
    public:
      Data();
      ~Data();
      std::string getData();
      void setData(std::string data);

    protected:

    private:
      boost::shared_ptr<char[]> containter; //ConstEV.blockSize
    };
  }
}

#endif