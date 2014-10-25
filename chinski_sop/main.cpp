#include <iostream>
#include <vector>
#include <string>
#include ".\sop\system\kernel.h"
#include ".\sop\logger\logger.h"

void funct(const std::vector<const std::string> & params)
{
  std::cout<<"funct executed with arguments:"<<std::endl;
  for(std::vector<const std::string>::size_type i=0;i<params.size();++i)
  {
    std::cout<<params[i]<<std::endl;
  }
}

class TestClass
{
  public:
    void method(const std::vector<const std::string> & params)
    {
      std::cout<<"meth executed with arguments:"<<std::endl;
      for(std::vector<const std::string>::size_type i=0;i<params.size();++i)
      {
        std::cout<<params[i]<<std::endl;
      }
    }
};

int main()
{
  sop::system::Kernel kernel(sop::logger::Logger::Level::FINEST);
  kernel.initialize();
  kernel.getShell()->registerCommand("funct",&funct);
  TestClass tc;
  kernel.getShell()->registerCommand(std::string("meth"),&TestClass::method,&tc);
  kernel.run();
  int x;
  std::cin>>x;
}