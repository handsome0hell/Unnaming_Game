// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include "CinInput.h"
#include <functional>
#include <iostream>
#include "./cininput_cmake.h"
CININPUT_EXPORT CinInput::~CinInput() {}
CININPUT_EXPORT void CinInput::HandleInput() {
  char com;
  std::cin >> com;
  auto function_finder = function_for_key_.find(com);
  if (function_finder != function_for_key_.end()) {
    (function_finder->second)();
  } else {
    null_function_();
  }
}
CININPUT_EXPORT void CinInput::BindKey(const char& key,
                                       const std::function< void() > function) {
  function_for_key_[key] = function;
}
CININPUT_EXPORT CinInput_ref CinInput::CreateCinInput(
                                 const std::function< void() > function) {
  return CinInput_ref(new CinInput(function));
}
CININPUT_NO_EXPORT CinInput::CinInput(
                       const std::function< void() > null_function)
                           : null_function_(null_function) {}
