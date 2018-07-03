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

#ifndef WEBSTREAMER_INCLUDE_QT_INPUTPROCESSOR_QT_INPUT_PROCESSOR_HPP_
#define WEBSTREAMER_INCLUDE_QT_INPUTPROCESSOR_QT_INPUT_PROCESSOR_HPP_

#include <QtWidgets/QWidget>
#include <qt_inputprocessor/export.hpp>
#include <webstreamer/input_processor.hpp>

namespace webstreamer {

class QT_INPUTPROCESSOR_EXPORT QtInputProcessor
    : public AsynchronousInputProcessor {
 public:
  QtInputProcessor(QWidget* widget);
  virtual void ProcessMouseInput(const MouseEventData& event) override;
  virtual void ProcessKeyboardInput(const KeyboardEventData& event) override;

 private:
  QWidget* widget_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_QT_INPUTPROCESSOR_QT_INPUT_PROCESSOR_HPP_
