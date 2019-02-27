#ifndef WEBSTREAMER_CUSTOMHTTPACTIONHANDLER_HPP_
#define WEBSTREAMER_CUSTOMHTTPACTIONHANDLER_HPP_

#include <cstdint>
#include <memory>
#include <stdexcept>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "webstreamer/export.hpp"

namespace webstreamer
{
  class WEBSTREAMER_EXPORT AbstractHTTPActionHandler
  {
    public:
      virtual ~AbstractHTTPActionHandler ( ) = default;
      virtual bool handleActionRequest( Poco::Net::HTTPServerRequest & request_, 
                                        Poco::Net::HTTPServerResponse & response_ ) = 0;
  };

  class DummyHTTPActionHandler : public AbstractHTTPActionHandler
  {
    public:
      bool handleActionRequest( Poco::Net::HTTPServerRequest & request_, 
                                Poco::Net::HTTPServerResponse & response_ );
  };

  class WEBSTREAMER_EXPORT ActionHandlerManager
  {
    public:
      static ActionHandlerManager & getInstance ( );

      ~ActionHandlerManager ( );

      template < class T >
      bool registerActionHandler ( )
      {
        if( !std::is_base_of < AbstractHTTPActionHandler, T >::value )
        {
          std::string message = "actionHandlerManager: "
                                "Attempted to register a non "
                                "abstractHTTPActionHandler-derived delegate";
          throw std::runtime_error ( message.c_str ( ) );
          return false;
        }

        _delegate = std::make_unique < T > ( );
        return true;
      }

      bool handleActionRequest( Poco::Net::HTTPServerRequest & request_, 
                                Poco::Net::HTTPServerResponse & response_ );
    private:
      ActionHandlerManager ( );

      static ActionHandlerManager INSTANCE;
  
      std::unique_ptr<AbstractHTTPActionHandler> _delegate;
  };
}

#endif
