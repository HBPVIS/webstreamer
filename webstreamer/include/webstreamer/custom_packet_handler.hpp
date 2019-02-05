#ifndef WEBSTREAMER_CUSTOMPACKETHANDLER_HPP_
#define WEBSTREAMER_CUSTOMPACKETHANDLER_HPP_

#include <cstdint>
#include <stdexcept>
#include <webstreamer/event.hpp>
#include <webstreamer/client.hpp>
#include <webstreamer/export.hpp>

namespace webstreamer
{
  class WEBSTREAMER_EXPORT AbstractPacketHandler
  {
    public:
      virtual ~AbstractPacketHandler ( );
      virtual Event::Ptr decodePacket(const void * data, std::size_t sizeBytes) = 0;
      virtual bool handlePacket(Client & client, Event::Ptr & event) = 0;
  };

  class WEBSTREAMER_EXPORT PacketHandlerManager
  {
    private:
      static PacketHandlerManager INSTANCE;
    public:
      static PacketHandlerManager & getInstance();
    private:
      std::unique_ptr<AbstractPacketHandler> delegate;
    private:
      PacketHandlerManager();
    public:
      ~PacketHandlerManager();
    
      template<class T>
      bool registerPacketHandler()
      {
        if(!std::is_base_of<AbstractPacketHandler, T>::value)
        {
          throw std::runtime_error("PacketHandlerManager: Attempted to register a non AbstractPacketHandler-derived delegate");
          return false;
        }

        delegate = std::make_unique<T>();
        return true;
      }

      Event::Ptr decodePacket(const void * data, const std::size_t & sizeBytes);
      bool handlePacket(Client & client, Event::Ptr & event);
  };
}

#endif
