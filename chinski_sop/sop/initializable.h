#ifndef _SOP_INITIALIZABLE_H_
#define _SOP_INITIALIZABLE_H_

namespace sop
{
  class Initializable
  {
    public:
      virtual void initialize() = 0;
    protected:
    private:
  };
}

#endif