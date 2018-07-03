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

#include <cassert>
#include <unordered_map>
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <webstreamer/event.hpp>
#include <webstreamer/log.hpp>
#include <qt_inputprocessor/qt_inputprocessor.hpp>

namespace webstreamer {

namespace {

Qt::KeyboardModifiers ParseKeyboardModifiers(ModifierKeys modifiers) {
  Qt::KeyboardModifiers result = Qt::NoModifier;
  if (CheckForFlags(modifiers, ModifierKeys::ALT)) {
    result |= Qt::AltModifier;
  }
  if (CheckForFlags(modifiers, ModifierKeys::CONTROL)) {
    result |= Qt::ControlModifier;
  }
  if (CheckForFlags(modifiers, ModifierKeys::META)) {
    result |= Qt::MetaModifier;
  }
  if (CheckForFlags(modifiers, ModifierKeys::SHIFT)) {
    result |= Qt::ShiftModifier;
  }
  return result;
}

Qt::MouseButton ParseMouseButton(MouseButton button) {
  switch (button) {
    case MouseButton::PRIMARY:   return Qt::LeftButton;
    case MouseButton::SECONDARY: return Qt::RightButton;
    case MouseButton::AUXILIARY: return Qt::MiddleButton;
    case MouseButton::FORTH:     return Qt::XButton1;  // i.e. QT::BackButton
    case MouseButton::FIFTH:     return Qt::XButton2;  // i.e. Qt::ForwardButton
  }
}

Qt::MouseButtons ParseMouseButtons(MouseButtons button) {
  Qt::MouseButtons result = Qt::NoButton;
  if (CheckForFlags(button, MouseButtons::PRIMARY)) {
    result |= Qt::LeftButton;
  }
  if (CheckForFlags(button, MouseButtons::SECONDARY)) {
    result |= Qt::RightButton;
  }
  if (CheckForFlags(button, MouseButtons::AUXILIARY)) {
    result |= Qt::MiddleButton;
  }
  if (CheckForFlags(button, MouseButtons::FORTH)) {
    result |= Qt::XButton1;  // i.e. QT::BackButton
  }
  if (CheckForFlags(button, MouseButtons::FIFTH)) {
    result |= Qt::XButton2;  // i.e. Qt::ForwardButton
  }
  return result;
}

int ParseKey(const std::string& code) {
  static const std::unordered_map<std::string, int> code_to_qt_key = {
    { "KeyA", Qt::Key_A },
    { "KeyB", Qt::Key_B },
    { "KeyC", Qt::Key_C },
    { "KeyD", Qt::Key_D },
    { "KeyE", Qt::Key_E },
    { "KeyF", Qt::Key_F },
    { "KeyG", Qt::Key_G },
    { "KeyH", Qt::Key_H },
    { "KeyI", Qt::Key_I },
    { "KeyJ", Qt::Key_J },
    { "KeyK", Qt::Key_K },
    { "KeyL", Qt::Key_L },
    { "KeyM", Qt::Key_M },
    { "KeyN", Qt::Key_N },
    { "KeyO", Qt::Key_O },
    { "KeyP", Qt::Key_P },
    { "KeyQ", Qt::Key_Q },
    { "KeyR", Qt::Key_R },
    { "KeyS", Qt::Key_S },
    { "KeyT", Qt::Key_T },
    { "KeyU", Qt::Key_U },
    { "KeyV", Qt::Key_V },
    { "KeyW", Qt::Key_W },
    { "KeyX", Qt::Key_X },
    { "KeyY", Qt::Key_Y },
    { "KeyZ", Qt::Key_Z },
    { "Digit0", Qt::Key_0 },
    { "Digit1", Qt::Key_1 },
    { "Digit2", Qt::Key_2 },
    { "Digit3", Qt::Key_3 },
    { "Digit4", Qt::Key_4 },
    { "Digit5", Qt::Key_5 },
    { "Digit6", Qt::Key_6 },
    { "Digit7", Qt::Key_7 },
    { "Digit8", Qt::Key_8 },
    { "Digit9", Qt::Key_9 },
    // Qt does not seem to have special 'numpad' key codes
    { "Numpad0", Qt::Key_0 },
    { "Numpad1", Qt::Key_1 },
    { "Numpad2", Qt::Key_2 },
    { "Numpad3", Qt::Key_3 },
    { "Numpad4", Qt::Key_4 },
    { "Numpad5", Qt::Key_5 },
    { "Numpad6", Qt::Key_6 },
    { "Numpad7", Qt::Key_7 },
    { "Numpad8", Qt::Key_8 },
    { "Numpad9", Qt::Key_9 },

    { "ArrowLeft", Qt::Key_Left},
    { "ArrowRight", Qt::Key_Right},
    { "ArrowUp", Qt::Key_Up},
    { "ArrowDown", Qt::Key_Down},
    { "ControlLeft", Qt::Key_Control},
    { "AltLeft", Qt::Key_Alt},
    { "AltRight", Qt::Key_Alt},
    { "ShiftLeft", Qt::Key_Shift},
    { "ShiftRight", Qt::Key_Shift},
    // TODO: this list is not complete
  };
  const auto it = code_to_qt_key.find(code);
  if (it == code_to_qt_key.end()) {
    LogD("Unknown key: ", code);
    return Qt::Key_unknown;
  }
  return it->second;
}

}  // namespace

QtInputProcessor::QtInputProcessor(QWidget* widget)
    : widget_(widget) {
  assert(widget != nullptr);
}

void QtInputProcessor::ProcessMouseInput(const MouseEventData& event_data) {
  // The position in MouseEventData is scaled to [0 - 1) i.e. [0 - uint16::max),
  // so reverse the scaling:
  constexpr double ONE_OVER_UINT16MAX =
      1.0 / std::numeric_limits<std::uint16_t>::max();
  const double x_factor = widget_->width() * ONE_OVER_UINT16MAX;
  const double y_factor = widget_->height() * ONE_OVER_UINT16MAX;
  const QPoint position_in_widget = {
    static_cast<int>(event_data.x * x_factor),
    static_cast<int>(event_data.y * y_factor)
  };

  QWidget* const widget_under_cursor = widget_->childAt(position_in_widget);
  if (widget_under_cursor == nullptr) {
    return;
  }
  const QPoint screen_position = widget_->mapToGlobal(position_in_widget);
  const QPoint local_position =
      widget_under_cursor->mapFrom(widget_, position_in_widget);

  const QEvent::Type qt_event_type = [](MouseAction action) {
    switch (action) {
      case MouseAction::BUTTON_DOWN:  return QEvent::MouseButtonPress;
      case MouseAction::BUTTON_UP:    return QEvent::MouseButtonRelease;
      case MouseAction::DOUBLE_CLICK: return QEvent::MouseButtonDblClick;
      case MouseAction::MOVE:         return QEvent::MouseMove;
    }
  }(event_data.action);

  QApplication::postEvent(widget_under_cursor, new QMouseEvent{
    qt_event_type,
    local_position,
    position_in_widget,
    screen_position,
    ParseMouseButton(event_data.button),
    ParseMouseButtons(event_data.buttons),
    ParseKeyboardModifiers(event_data.modifiers),
  });
}

void QtInputProcessor::ProcessKeyboardInput(
    const KeyboardEventData& event_data) {
  QWidget* const widget_with_keyboard_focus = QApplication::focusWidget();

  if (widget_with_keyboard_focus == nullptr) {
    return;
  }

  const std::string key(event_data.key);
  const std::string code(event_data.code);

  const QEvent::Type qt_event_type = [](KeyAction action) {
    switch (action) {
      case KeyAction::DOWN:  return QEvent::KeyPress;
      case KeyAction::UP:    return QEvent::KeyRelease;
      case KeyAction::PRESS: return QEvent::KeyPress;
    }
  }(event_data.action);

  QApplication::postEvent(widget_with_keyboard_focus, new QKeyEvent{
    qt_event_type,
    ParseKey(code),
    ParseKeyboardModifiers(event_data.modifiers),
    event_data.action == KeyAction::PRESS ? key.c_str() : "",
    event_data.repeat
  });
}

}  // namespace webstreamer
