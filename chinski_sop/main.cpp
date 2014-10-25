#include <iostream>
#include <vector>
#include <string>
#include ".\sop\system\kernel.h"
#include ".\sop\logger\logger.h"

std::string funct(const std::vector<const std::string> & params)
{
  std::cout<<"funct executed"<<std::endl;
  return "";
}

class TestClass
{
  public:
    std::string method(const std::vector<const std::string> & params)
    {
      std::cout<<"meth executed"<<std::endl;
      return "";
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