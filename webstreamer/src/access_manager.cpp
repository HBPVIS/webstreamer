#include <webstreamer/access_manager.hpp>

namespace webstreamer
{
  AccessManager AccessManager::_INSTANCE;

  AccessManager & AccessManager::getInstance ( )
  {
    return _INSTANCE;
  }

  AccessManager::AccessManager ( )
   : _enable ( true )
  {
  }

  void AccessManager::setAccessControlEnabled ( bool val_ )
  {
    _enable = val_;
  }

  void AccessManager::allowAddress ( const std::string & address_ )
  {
    if ( _enable )
    {
      std::unique_lock < std::mutex > lock ( _mtx );
      _accessControl.insert ( address_ );
    }
  }

  bool AccessManager::addressIsAllowed ( const std::string & address_ )
  {
    if ( _enable )
    {
      std::unique_lock < std::mutex > lock ( _mtx );
      auto it = _accessControl.find ( address_ );
      return it != _accessControl.end ( );
    }

    return true;
  }
}