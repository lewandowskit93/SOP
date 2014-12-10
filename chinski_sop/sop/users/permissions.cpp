#include ".\sop\users\permissions.h"

sop::users::Permissions::Permissions():
  user(kNone),
  group(kNone),
  others(kNone)
{
}


sop::users::Permissions::Permissions(bool for_directory):
  user(for_directory==true ? kRWX : kRW),
  group(for_directory==true ? kRWX : kRW),
  others(for_directory==true ? kRWX : kRW)
{
}

sop::users::Permissions::Permissions(permission_t user, permission_t group, permission_t others):
  user(user),
  group(group),
  others(others)
{
}


bool sop::users::PermissionsUtilities::isModeAllowed(permission_t permission,permission_t mode)
{
  return (permission & mode) == mode;
}

std::string sop::users::PermissionsUtilities::getRWXString(permission_t permission)
{
  switch(permission) //maybe bit &?
  {
      case Permissions::kRWX:
        {
          return "rwx";
          break;
        }
      case Permissions::kRW:
        {
          return "rw-";
          break;
        }
      case Permissions::kRX:
        {
          return "r-x";
          break;
        }
      case Permissions::kR:
        {
          return "r--";
          break;
        }
      case Permissions::kWX:
        {
          return "-wx";
          break;
        }
      case Permissions::kW:
        {
          return "-w-";
          break;
        }
      case Permissions::kX:
        {
          return "--x";
          break;
        }
      case Permissions::kNone:
      default:
        {
          return "---";
        }
  }
}

std::string sop::users::PermissionsUtilities::getRWXString(permission_t user, permission_t group, permission_t others)
{
  return getRWXString(user)+getRWXString(group)+getRWXString(others);
}

std::string sop::users::PermissionsUtilities::getRWXString(Permissions permissions)
{
  return getRWXString(permissions.user,permissions.group,permissions.others);
}

sop::users::PermissionsUtilities::PermissionsUtilities()
{
}

sop::users::permission_t sop::users::PermissionsUtilities::getFromRWXString(std::string permissions_s)
{
  if(permissions_s=="rwx")return Permissions::kRWX;
  if(permissions_s=="rw-")return Permissions::kRW;
  if(permissions_s=="r-x")return Permissions::kRX;
  if(permissions_s=="r--")return Permissions::kR;
  if(permissions_s=="-wx")return Permissions::kWX;
  if(permissions_s=="-w-")return Permissions::kW;
  if(permissions_s=="--x")return Permissions::kX;
  return 0;
}