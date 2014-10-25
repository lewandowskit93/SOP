#include <iostream>
#include ".\sop\system\kernel.h"
#include ".\sop\logger\logger.h"

int main()
{
  sop::system::Kernel kernel(sop::logger::Logger::Level::FINEST);
  kernel.initialize();
  kernel.run();
  int x;
  std::cin>>x;
}