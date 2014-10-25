#include <iostream>
#include ".\sop\system\system.h"
#include ".\sop\logger\logger.h"

int main()
{
  sop::system::System system(sop::logger::Logger::Level::FINEST);
  system.initializeSystem();
  system.runSystem();
  int x;
  std::cin>>x;
}