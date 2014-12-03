#include <iostream>
#include <string>
#include <cstdint>

class Process
{
public:
  int16_t PID;
  
protected:
  
  int16_t UID;
  int16_t GID;


private:
  int16_t PPID;

};