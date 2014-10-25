#ifndef _SOP_OBJECT_H_
#define _SOP_OBJECT_H_

#include <string>

namespace sop
{

  /*
    Simple Object class.
    Every class should inherit from it.
  */
  class Object
  {
    public:
      Object();
      virtual ~Object();

      /*
        Returns class name with leading namespaces.
        E.g. sop::Object

        Every class should override this function.
      */
      virtual std::string getClassName() const;
      
      /*
        Returns class name and value of pointer to this object.
        May be overriden if necessary.
      */
      virtual std::string toString() const;

    protected:

    private:

  };
}

#endif