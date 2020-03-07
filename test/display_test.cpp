#include "display.hpp"

#include <iostream>

using namespace led_driver;

int main() {
  LEDPins pins;
  pins.A = 4;
  pins.B = 17;
  pins.OE = 14;
  pins.Clk = 2;
  pins.LAT = 15;
  pins.Dr = 3;

  auto display = create_led_display(pins);

  Pixels pixels{1};
  for (auto &part : pixels.pixels) {
    part[1] = true;
    part[2] = true;
  }

  display->set_pixels(std::move(pixels));

  std::cin.get();
}