#ifndef _SOP_USERS_GROUPS_MANAGER_H_
#define _SOP_USERS_GROUPS_MANAGER_H_

#include <list>
#include <string>
#include <boost\shared_ptr.hpp>
#include <boost\regex.hpp>
#include ".\sop\users\id_definitions.h"
#include ".\sop\object.h"
#include ".\sop\users\group.h"

//#define GROUP_FILE_INTEGRATED

namespace sop
{
  namespace users
  {

    class Module;

    class GroupsManager : public Object
    {
      public:
        explicit GroupsManager(Module *module);
        virtual ~GroupsManager();
        virtual std::string getClassName() const;
        bool addGroup(boost::shared_ptr<Group> group);
        bool addGroup(const Group & group);
        bool deleteGroup(const std::string & group_name);
        boost::shared_ptr<Group> findGroup(gid_t gid);
        boost::shared_ptr<Group> findGroup(const std::string & group_name);
        gid_t getNextFreeGID(gid_t greater_than = kMax_system_gid);
        bool isGIDFree(gid_t gid);
        bool isGroupNameFree(const std::string & group_name);
        std::list<boost::shared_ptr<Group>> getGroupsList();
        void loadGroupsFromFile(const std::string & filename);
        void saveGroupsToFile(const std::string & filename);
        static bool isGroupNameValid(const std::string & group_name);
        static const boost::regex group_name_regex;

      protected:

        Module *_module;
        std::list<boost::shared_ptr<Group>> _groups_list;
      private:
        
    };
  }
}

#endif