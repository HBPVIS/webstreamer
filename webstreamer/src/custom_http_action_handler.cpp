#include "webstreamer/custom_http_action_handler.hpp"

#include <iostream>

namespace webstreamer
{
  bool 
  DummyHTTPActionHandler::handleActionRequest( Poco::Net::HTTPServerRequest & request_, 
                                               Poco::Net::HTTPServerResponse & response_ )
  {
    std::cout << "Received request " << request_.getURI ( ) << std::endl;
    response_.setStatus ( Poco::Net::HTTPResponse::HTTP_BAD_REQUEST );
    return false;
  }

  ActionHandlerManager ActionHandlerManager::INSTANCE;

  ActionHandlerManager & ActionHandlerManager::getInstance ( )
  {
    return INSTANCE;
  }

  ActionHandlerManager::ActionHandlerManager ( )
  {
    registerActionHandler < DummyHTTPActionHandler > ( );
  }

  ActionHandlerManager::~ActionHandlerManager ( )
  {
  }

  bool 
  ActionHandlerManager::handleActionRequest( Poco::Net::HTTPServerRequest & request_, 
                                             Poco::Net::HTTPServerResponse & response_ )
  {
    return _delegate.get ( )->handleActionRequest ( request_, response_ );
  }
}
