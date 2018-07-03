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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_STD_FUNCTION_DELEGATE_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_STD_FUNCTION_DELEGATE_HPP_

#include <functional>
#pragma warning(push, 0)
#pragma warning(disable: 4702)
#include "Poco/AbstractDelegate.h"
#pragma warning(pop)

namespace webstreamer {

template <class TArgs>
class StdFunctionDelegate : public Poco::AbstractDelegate<TArgs> {
  typedef std::function<void(const void*, TArgs&)> FunctionType;

 public:
  StdFunctionDelegate(FunctionType function) : function_(function) {}

  inline bool notify(const void* sender, TArgs& arguments) override {
    if (function_) {
      function_(sender, arguments);
      return true;
    } else {
      return false;
    }
  }

  inline bool equals(
      const Poco::AbstractDelegate<TArgs>& other) const override {
    auto other_std_function_ =
        dynamic_cast<const StdFunctionDelegate<TArgs>*>(&other);
    if (other_std_function_ == nullptr) {
      return false;
    } else {
      assert(false && "Cannot compare std::function objects.");
      return false;
    }
  }

  inline Poco::AbstractDelegate<TArgs>* clone() const override {
    return new StdFunctionDelegate(*this);
  }

  inline void disable() override { function_ = nullptr; }

 private:
  FunctionType function_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_STD_FUNCTION_DELEGATE_HPP_