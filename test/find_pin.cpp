#include "display.hpp"

#include <chrono>
#include <iostream>

int main(int argc, char **argv) {
  std::vector<int> pins;

  for (int i = 1; i < argc; i++) {
    pins.push_back(std::atoi(argv[i]));
  }

  led_driver::SetPins set;

  while (true) {
    for (auto pin : pins) {
      set(pin, true);
    }
    set.apply();

    for (auto pin : pins) {
      set(pin, false);
    }
    set.apply();
  }
}