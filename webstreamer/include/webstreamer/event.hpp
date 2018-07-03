//------------------------------------------------------------------------------
// Web Streamer
//
// Copyright (c) 2017 RWTH Aachen University, Germany,
// Virtual Reality & Immersive Visualization Group.
//------------------------------------------------------------------------------
//                                 License
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_EVENT_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_EVENT_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include "webstreamer/export.hpp"
#include "webstreamer/flags.hpp"

namespace webstreamer {

enum class EventType : std::uint8_t {
  UNKNOWN = 0x0,

  // 0x10 - 0x1F: Stream control events
  STREAM_CONFIG_CHANGED = 0x10,
  CHANGE_CODEC = 0x11,

  // 0x30 - 0x3F: User control events
  PLAY = 0x30,
  STOP = 0x31,
  AQUIRE_INPUT = 0x32,
  RELEASE_INPUT = 0x33,

  // 0x50 - 0x5F: Input events
  MOUSE_INPUT = 0x50,
  KEYBOARD_INPUT = 0x51,
};

class WEBSTREAMER_EXPORT Event {
 public:
  typedef std::unique_ptr<Event> Ptr;

  explicit inline Event(EventType type) : type_(type) {}
  Event(const void* data, std::size_t size_in_bytes);
  virtual ~Event() = default;

  inline EventType type() const { return type_; }

  virtual std::vector<std::uint8_t> Serialize() const;
  virtual std::string ToString() const;

 private:
  EventType type_;
};

Event::Ptr DeserializeEvent(const void* data, std::size_t size_in_bytes);

// enum class ModifierKeys : std::uint8_t {
//   ALT = 1 << 0,
//   CONTROL = 1 << 1,
//   META = 1 << 2,
//   SHIFT = 1 << 3
// };
// WEBSTREAMER_CONVERT_TYPED_ENUM_TO_FLAGS(ModifierKeys);

// #ifdef DOUBLE_CLICK
// #undef DOUBLE_CLICK
// #endif
// enum class MouseAction : std::uint8_t {
//   MOVE,
//   BUTTON_DOWN,
//   BUTTON_UP,
//   DOUBLE_CLICK,
// };

// enum class MouseButton : std::uint8_t {
//   PRIMARY = 0,
//   AUXILIARY = 1,
//   SECONDARY = 2,
//   FORTH = 3,
//   FIFTH = 4,
// };

// enum class MouseButtons : std::uint8_t {
//   PRIMARY = 1 << 0,
//   SECONDARY = 1 << 1,
//   AUXILIARY = 1 << 2,
//   FORTH = 1 << 3,
//   FIFTH = 1 << 4,
// };
// WEBSTREAMER_CONVERT_TYPED_ENUM_TO_FLAGS(MouseButtons);

// enum class KeyAction : std::uint8_t {
//   DOWN,
//   UP,
//   PRESS,
// };

// struct WEBSTREAMER_EXPORT EventData {
//   EventType type;
// };
// static_assert(sizeof(EventData) == 1, "Padding error");

// struct WEBSTREAMER_EXPORT InputEventData : public EventData {
//   ModifierKeys modifiers;
//   std::uint16_t padding;
//   std::uint32_t input_timestamp;
// };
// static_assert(sizeof(InputEventData) == sizeof(EventData) + 7, "Padding
// error");

// struct WEBSTREAMER_EXPORT MouseEventData : public InputEventData {
//   MouseAction action;
//   MouseButton button;
//   MouseButtons buttons;
//   std::uint8_t padding;
//   std::uint16_t x;
//   std::uint16_t y;
// };
// static_assert(sizeof(MouseEventData) == sizeof(InputEventData) + 8,
//               "Padding error");

// struct WEBSTREAMER_EXPORT KeyboardEventData : public InputEventData {
//   static constexpr int MAX_KEY_CHARACTERS_COUNT = 16;
//   static constexpr int MAX_CODE_CHARACTERS_COUNT = 16;

//   KeyAction action;
//   std::uint8_t repeat;  // uint8 because sizeof(bool) is not required to be 1
//   std::uint16_t padding;
//   // Encoded in UTF-8, see https://w3c.github.io/uievents-key/
//   char key[MAX_KEY_CHARACTERS_COUNT];
//   // Encoded in UTF-8, see https://w3c.github.io/uievents-code/
//   char code[MAX_CODE_CHARACTERS_COUNT];
// };
// static_assert(sizeof(KeyboardEventData) ==
//                   sizeof(InputEventData) +
//                       KeyboardEventData::MAX_KEY_CHARACTERS_COUNT +
//                       KeyboardEventData::MAX_CODE_CHARACTERS_COUNT + 4,
//               "Padding error");

// struct WEBSTREAMER_EXPORT ResolutionEventData : public EventData {
//   std::uint8_t padding;
//   std::uint16_t width;
//   std::uint16_t height;
// };
// static_assert(sizeof(ResolutionEventData) == sizeof(EventData) + 5,
//               "Padding error");

// struct WEBSTREAMER_EXPORT ChangeCodecEventData : public EventData {
//   std::uint8_t codec;
//   char options[126];
// };
// static_assert(sizeof(ChangeCodecEventData) == sizeof(EventData) + 127,
//               "Padding error");

// union WEBSTREAMER_EXPORT Event {
//   EventType type;
//   MouseEventData mouse;
//   KeyboardEventData keyboard;
//   ResolutionEventData resolution;
//   ChangeCodecEventData change_codec;
// };
// static_assert(offsetof(EventData, type) == 0, "Non matching type offsets");
// static_assert(std::is_trivially_copyable<Event>::value,
//               "Event must be trivially copyable");

// /** Parses an event from a chunk of memory. */
// Event ParseEventFromMemory(const void* buffer,
//                            std::size_t buffer_size_in_bytes);
// std::size_t GetEventDataSize(const Event& event);

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_EVENT_HPP_
