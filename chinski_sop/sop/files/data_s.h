#ifndef _SOP_FILES_DATA_S_H_
#define _SOP_FILES_DATA_S_H_

#include <iostream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "sop/files/data.h"
#include "sop/files/block.h"
#include "sop/logger/logger.h"

namespace sop
{
  namespace files
  {
    class Data;
    class Data_s
    {
    public:
      Data_s(Data* ptr, sop::logger::Logger* logger);
      ~Data_s();
      
      sop::files::Block* readData(std::vector<char>);
      std::vector<char> writeData();

    protected:

    private:
      boost::shared_ptr<Data> data_p;
      sop::logger::Logger* logger;
    };
  }
}

#endif