#include ".\sop\system\system.h"
#include <iostream>

int main()
{
  sop::system::System system;
  system.initializeSystem();
  system.runSystem();
  int x;
  std::cin>>x;
}