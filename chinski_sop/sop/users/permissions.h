#ifndef _SOP_USERS_PERMISSIONS_H_
#define _SOP_USERS_PERMISSIONS_H_

#include <stdint.h>
#include <string>

namespace sop
{
  namespace users
  {
    typedef uint8_t permission_t;

    struct Permissions
    {
      static const permission_t kRWX=7;
      static const permission_t kRW=6;
      static const permission_t kRX=5;
      static const permission_t kR=4;
      static const permission_t kWX=3;
      static const permission_t kW=2;
      static const permission_t kX=1;
      static const permission_t kNone=0;

      Permissions(bool for_directory); //dir 777 file 666
      Permissions(permission_t user, permission_t group, permission_t others);

      permission_t user;
      permission_t group;
      permission_t others;
    };

    class PermissionsUtilities
    {
      public:
        static std::string getRWXString(permission_t permission);
        static std::string getRWXString(permission_t user, permission_t group, permission_t others);
        static std::string getRWXString(Permissions permissions);
      protected:
      private:
        PermissionsUtilities();
    };
  }
}

#endif