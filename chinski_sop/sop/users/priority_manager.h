#ifndef _SOP_USERS_PRIORITY_MANAGER_H_
#define _SOP_USERS_PRIORITY_MANAGER_H_

#include <map>
#include ".\sop\object.h"
#include ".\sop\users\id_definitions.h"

//#define PRIORITIES_FILE_INTEGRATED


namespace sop
{
  namespace users
  {

    class Module;

    class PriorityManager : public Object
    {
      public:
        explicit PriorityManager(Module *module);
        virtual ~PriorityManager();
        virtual std::string getClassName() const;

        int8_t getUserPriority(uid_t uid);
        int8_t getGroupPriority(gid_t gid);
        void setUserPriority(uid_t uid, priority_t priority);
        void setGroupPriority(gid_t gid, priority_t priority);
        
        // does nothing
        void removeUserPriorityEntry(uid_t uid); 
        void removeGroupPriorityEntry(gid_t gid);

        void loadPrioritiesFromFile(const std::string & filename);
        void savePrioritiesToFile(const std::string & filename);


      protected:
        Module *_module;
        std::map<gid_t,priority_t> priorities;
      private:

    };
  }
}

#endif