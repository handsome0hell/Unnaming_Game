#include "CinInput.h"
#include <iostream>
#include <cininput_export.h>
#include <functional>
CININPUT_EXPORT void CinInput::HandleInput() {
  char com;
  std::cin >> com;
  auto function_finder = function_for_key_.find(com);
  if (function_finder != function_for_key_.end()) {
    (function_finder -> second)();
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
                           : null_function_(null_function) {};
