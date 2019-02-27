#include "webstreamer/custom_packet_handler.hpp"

namespace webstreamer
{
  AbstractPacketHandler::~AbstractPacketHandler ( )
  {
  }

  PacketHandlerManager PacketHandlerManager::INSTANCE;

  PacketHandlerManager & PacketHandlerManager::getInstance()
  {
    return INSTANCE;
  }

  PacketHandlerManager::PacketHandlerManager()
  {
  }

  PacketHandlerManager::~PacketHandlerManager()
  {
  }

  Event::Ptr PacketHandlerManager::decodePacket(const void * data, const std::size_t & sizeBytes)
  {
    if (delegate.get() != nullptr)
    {
      return delegate.get()->decodePacket(data, sizeBytes);
    }

    return std::make_unique<Event>(EventType::UNKNOWN);
  }

  bool PacketHandlerManager::handlePacket(Client & client, Event::Ptr & event)
  {
    if (delegate.get() != nullptr)
    {
      return delegate.get()->handlePacket(client, event);
    }

    return false;
  }
}
