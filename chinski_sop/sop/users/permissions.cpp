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