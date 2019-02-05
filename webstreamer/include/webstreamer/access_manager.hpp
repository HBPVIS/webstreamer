#ifndef WEBSTREAMER_ACCESSMANAGER_HPP_
#define WEBSTREAMER_ACCESSMANAGER_HPP_

#include <string>
#include <set>
#include <mutex>

#include <webstreamer/export.hpp>

namespace webstreamer
{
  class WEBSTREAMER_EXPORT AccessManager
  {
    public:
      static AccessManager & getInstance ( );

      AccessManager ( );

      void setAccessControlEnabled ( bool val_ );
      bool addressIsAllowed ( const std::string & address_ );
      void allowAddress ( const std::string & address_ );

    private:
      static AccessManager _INSTANCE;

      bool _enable;
      std::mutex _mtx;
      std::set < std::string > _accessControl;
  };
}

#endif